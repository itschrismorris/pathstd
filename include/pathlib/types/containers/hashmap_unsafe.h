/*
  Documentation: https://www.path.blog/docs/hashmap.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/containers/vector_unsafe.h"
#include "pathlib/types/string/size_of.h"
#include "pathlib/types/string/long_string_unsafe.h"

namespace Pathlib {

//---
template <typename K, 
          typename V, 
          u64 RESERVE_CAPACITY>
struct HashmapUnsafe
{
  //---
  static_assert(Math::is_pot(RESERVE_CAPACITY) && (RESERVE_CAPACITY >= 8), "Hashmap RESERVE_CAPACITY must be a power of two, greater or equal to 8.");

  //---
  static u32 constexpr MAX_REHASHES = 2;
  static u32 constexpr DIGEST_MASK = 0x0FFFFFFF;
  static u32 constexpr OCCUPIED_AND_DIGEST_MASK = 0x1FFFFFFF;
  static u32 constexpr EMPTY_SLOT = 0x7FFFFFFF;
  static u32 constexpr NEW_KV = Types::U32_MAX;
  static u32 constexpr NEW_HASH = Types::U32_MAX;
  static u32 constexpr DISTANCE_SHIFT = 29;

  //---
  u32 capacity;
  u32 max_probe_length;
  u32* slot_kv_index;
  u32* slot_distance_digest;
  VectorUnsafe<K, RESERVE_CAPACITY> keys;
  VectorUnsafe<V, RESERVE_CAPACITY> values;
  VectorUnsafe<u32, RESERVE_CAPACITY> kv_slot_lookup;

  //---
  HashmapUnsafe()
  {
    capacity = RESERVE_CAPACITY;
    max_probe_length = 1 + (Math::log2(capacity) >> 2);
    slot_kv_index = (u32*)malloc_unsafe(sizeof(u32) * capacity);
    slot_distance_digest = (u32*)malloc_unsafe(sizeof(u32) * capacity);
    I8 empty_slot = I8_SET1(EMPTY_SLOT);
    for (u32 r = 0; r < (capacity >> 3); ++r) {
      I8_STORE(&((I8*)slot_distance_digest)[r], empty_slot);
    }
  }

  //---
  ~HashmapUnsafe()
  {
    if (slot_kv_index) {
      free_unsafe((void**)&slot_kv_index);
    }
    if (slot_distance_digest) {
      free_unsafe((void**)&slot_distance_digest);
    }
  }
  
  //---
  template <typename T>
  u32 hash(T key)
  {
    if constexpr (IS_INTEGRAL(T) || IS_FLOAT(T)) {
      return Math::hash(key);
    } else if constexpr (IS_SHORT_STRING(T)) {
      return key.hash();
    } else if constexpr (IS_LONG_STRING(T)) {
      return key.hash();
    } else if constexpr (SAME_TYPE(T, const utf8*)) {
      return LongStringUnsafe<64>::hash(key);
    } else {
      static_assert(false, "Unsupported type used for hashmap key.");
    }
  }

  //---
  inline V* find(const K& key,
                 u32 existing_hash = NEW_HASH,
                 u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == NEW_HASH) ? hash(key) : existing_hash;
    u32 slot_index = (key_hash & (capacity - 1));
    for (u32 i = 0; i < max_probe_length; ++i) {
      slot_index = Math::min((u32)capacity - 8, slot_index);
      I8 key_digest = I8_SET1(hash(key_hash) & DIGEST_MASK);
      I8 digests = I8_AND(I8_LOADU(&slot_distance_digest[slot_index]), I8_SET1(OCCUPIED_AND_DIGEST_MASK));
      u32 digest_mask = I8_MOVEMASK(I8_CMP_EQ(key_digest, digests));
      while (digest_mask) {
        u32 distance = Math::lsb_set(digest_mask) >> 2;
        u32 kv_index = slot_kv_index[slot_index + distance];
        if (keys[kv_index] == key) {
          return &values[kv_index];
        }
        digest_mask ^= (0xF << (distance << 2));
      }
      slot_index = (slot_index + 8) & (capacity - 1);
    }
    if (hash_count < MAX_REHASHES) {
      return find(key, hash(key_hash), hash_count + 1);
    }
    return nullptr;
  }

  //---
  V* operator [](const K& key)
  {
    return find(key);
  }

  //---
  inline bool insert(const K& key,
                     const V& value,
                     u32 kv_index = NEW_KV,
                     u32 existing_hash = NEW_HASH,
                     u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == NEW_HASH) ? hash(key) : existing_hash;
    u32 slot_index = key_hash & (capacity - 1);
    for (u32 i = 0; i < max_probe_length; ++i) {
      slot_index = Math::min((u32)capacity - 8, slot_index);
      I8 slots = I8_LOADU(&slot_distance_digest[slot_index]);
      I8 slot_distances = I8_SHIFTR(slots, 29);
      I8 empty_slot = I8_SET1(EMPTY_SLOT);
      I8 distance_check = I8_SET(0, 1, 2, 3, 4, 5, 6, 7);
      u32 empty_mask = I8_MOVEMASK(I8_CMP_EQ(slots, empty_slot));
      u32 swap_mask = I8_MOVEMASK(I8_CMP_GT(slot_distances, distance_check));
      if (empty_mask || swap_mask) {
        u32 empty_distance = Math::lsb_set(empty_mask) >> 2;
        u32 swap_distance = Math::lsb_set(swap_mask) >> 2;
        if (empty_distance <= swap_distance) {
          u32 insert_index = slot_index + empty_distance;
          if (kv_index == NEW_KV) {
            *keys.emplace_back(1) = key;
            *values.emplace_back(1) = value;
            kv_slot_lookup.emplace_back(1);
            kv_index = values.count - 1;
          }
          slot_kv_index[insert_index] = kv_index;
          slot_distance_digest[insert_index] = (empty_distance << DISTANCE_SHIFT) | (hash(key_hash) & DIGEST_MASK);
          kv_slot_lookup[kv_index] = insert_index;
          return true;
        } else {
          u32 insert_index = slot_index + swap_distance;
          if (kv_index == NEW_KV) {
            *keys.emplace_back(1) = key;
            *values.emplace_back(1) = value;
            kv_slot_lookup.emplace_back(1);
            kv_index = values.count - 1;
          }
          u32 swap_kv_index = slot_kv_index[insert_index];
          K& swap_key = keys[swap_kv_index];
          V& swap_value = values[swap_kv_index];
          slot_kv_index[insert_index] = kv_index;
          slot_distance_digest[insert_index] = (swap_distance << DISTANCE_SHIFT) | (hash(key_hash) & DIGEST_MASK);
          kv_slot_lookup[kv_index] = insert_index;
          return insert(swap_key, swap_value, swap_kv_index);
        }
      }
      slot_index = (slot_index + 8) & (capacity - 1);
    }
    if (hash_count < MAX_REHASHES) {
      if (kv_index == NEW_KV) {
        return insert(key, value, kv_index, hash(key_hash), hash_count + 1);
      } else {
        return insert(keys[kv_index], values[kv_index], kv_index, hash(key_hash), hash_count + 1);
      }
    }
    if (kv_index == NEW_KV) {
      *keys.emplace_back(1) = key;
      *values.emplace_back(1) = value;
      kv_slot_lookup.emplace_back(1);
    }
    return rebuild_larger();
  }

  //---
  inline bool remove(const K& key,
                     u32 existing_hash = Types::U32_MAX,
                     u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == Types::U32_MAX) ? hash(key) : existing_hash;
    u32 slot_index = (key_hash & (capacity - 1));
    for (u32 i = 0; i < max_probe_length; ++i) {
      slot_index = Math::min((u32)capacity - 8, slot_index);
      I8 key_digest = I8_SET1(hash(key_hash) & DIGEST_MASK);
      I8 digests = I8_AND(I8_LOADU(&slot_distance_digest[slot_index]), I8_SET1(OCCUPIED_AND_DIGEST_MASK));
      u32 digest_mask = I8_MOVEMASK(I8_CMP_EQ(key_digest, digests));
      while (digest_mask) {
        u32 distance = Math::lsb_set(digest_mask) >> 2;
        u32 kv_index = slot_kv_index[slot_index + distance];
        if (keys[kv_index] == key) {
          slot_kv_index[kv_slot_lookup[kv_slot_lookup.count - 1]] = kv_index;
          slot_distance_digest[slot_index + distance] = EMPTY_SLOT;
          keys.remove(kv_index, 1);
          values.remove(kv_index, 1);
          kv_slot_lookup.remove(kv_index, 1);
          return true;
        }
        digest_mask ^= (0xF << (distance << 2));
      }
      slot_index = (slot_index + 8) & (capacity - 1);
    }
    if (hash_count < MAX_REHASHES) {
      return remove(key, hash(key_hash), hash_count + 1);
    }
    return false;
  }

  //---
  inline bool rebuild_larger()
  {
    console.write(load_factor());
    capacity <<= 1;
    max_probe_length = 1 + (Math::log2(capacity) >> 2);
    slot_kv_index = (u32*)realloc_unsafe(slot_kv_index, sizeof(u32) * capacity);
    slot_distance_digest = (u32*)realloc_unsafe(slot_distance_digest, sizeof(u32) * capacity);
    I8 empty_slot = I8_SET1(EMPTY_SLOT);
    for (u32 r = 0; r < (capacity >> 3); ++r) {
      I8_STORE(&((I8*)slot_distance_digest)[r], empty_slot);
    }
    for (u32 v = 0; v < values.count; ++v) {
      insert(keys[v], values[v], v);
    }
    return true;
  }
 
  //---
  inline f32 load_factor()
  {
    u32 used_slots = 0;
    for (u32 b = 0; b < capacity; ++b) {
      used_slots += (slot_distance_digest[b] != EMPTY_SLOT);
    }
    return ((f32)used_slots / (f32)capacity);
  }
};
}