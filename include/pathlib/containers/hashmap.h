/*
  Documentation: https://www.path.blog/docs/hashmap.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/containers/vector_unsafe.h"
#include "pathlib/string/size_of.h"
#include "pathlib/string/long_string_unsafe.h"

namespace Pathlib {

//---
template <typename K, 
          typename V, 
          u64 RESERVE_CAPACITY>
struct Hashmap
{
  //---
  static_assert(Math::is_pot(RESERVE_CAPACITY) && (RESERVE_CAPACITY >= 8), 
                "Hashmap RESERVE_CAPACITY must be a power of two, greater or equal to 8.");

  //---
  static u32 constexpr MAX_REHASHES = 2;
  static u32 constexpr DIGEST_MASK = 0x0FFFFFFF;
  static u32 constexpr OCCUPIED_AND_DIGEST_MASK = 0x1FFFFFFF;
  static u32 constexpr EMPTY_SLOT = 0x7FFFFFFF;
  static u32 constexpr NEW_KV = Types::U32_MAX;
  static u32 constexpr NEW_HASH = Types::U32_MAX;
  static u32 constexpr DISTANCE_SHIFT = 29;

private:
  //---
  u32 _capacity;
  u32 _max_probe_length;
  u32* _slot_kv_index;
  u32* _slot_distance_digest;
  VectorUnsafe<K, RESERVE_CAPACITY> _keys;
  VectorUnsafe<V, RESERVE_CAPACITY> _values;
  VectorUnsafe<u32, RESERVE_CAPACITY> _kv_slot_lookup;

public:
  //---
  DISALLOW_COPY(Hashmap);

  //---
  Hashmap(const utf8* name)
  {
    _capacity = RESERVE_CAPACITY;
    _max_probe_length = 1 + (Math::log2(_capacity) >> 2);
    _slot_kv_index = (u32*)malloc_unsafe(sizeof(u32) * _capacity, 
                                        ShortStringUnsafe<96>(name, u8"::_slot_kv_index")._str);
    _slot_distance_digest = (u32*)malloc_unsafe(sizeof(u32) * _capacity, 
                                               ShortStringUnsafe<96>(name, u8"::_slot_distance_digest")._str);
    I8 empty_slot = I8_SET1(EMPTY_SLOT);
    for (u32 r = 0; r < (_capacity >> 3); ++r) {
      I8_STORE(&((I8*)_slot_distance_digest)[r], empty_slot);
    }
  }

  //---
  ~Hashmap()
  {
    if (_slot_kv_index) {
      free_unsafe((void**)&_slot_kv_index);
    }
    if (_slot_distance_digest) {
      free_unsafe((void**)&_slot_distance_digest);
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
  inline SafePtr<V> find(const K& key,
                         u32 existing_hash = NEW_HASH,
                         u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == NEW_HASH) ? hash(key) : existing_hash;
    u32 slot_index = (key_hash & (_capacity - 1));
    for (u32 i = 0; i < _max_probe_length; ++i) {
      slot_index = Math::min((u32)_capacity - 8, slot_index);
      I8 key_digest = I8_SET1(hash(key_hash) & DIGEST_MASK);
      I8 digests = I8_AND(I8_LOADU(&_slot_distance_digest[slot_index]), 
                          I8_SET1(OCCUPIED_AND_DIGEST_MASK));
      u32 digest_mask = I8_MOVEMASK(I8_CMP_EQ(key_digest, digests));
      while (digest_mask) {
        u32 distance = Math::lsb_set(digest_mask) >> 2;
        u32 kv_index = _slot_kv_index[slot_index + distance];
        if (_keys[kv_index] == key) {
          return SafePtr<V>(&_values[kv_index]);
        }
        digest_mask ^= (0xF << (distance << 2));
      }
      slot_index = (slot_index + 8) & (_capacity - 1);
    }
    if (hash_count < MAX_REHASHES) {
      return find(key, hash(key_hash), hash_count + 1);
    }
    return nullptr;
  }

  //---
  SafePtr<V> operator [](const K& key)
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
    u32 slot_index = key_hash & (_capacity - 1);
    for (u32 i = 0; i < _max_probe_length; ++i) {
      slot_index = Math::min((u32)_capacity - 8, slot_index);
      I8 slots = I8_LOADU(&_slot_distance_digest[slot_index]);
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
            *_keys.emplace_back(1) = key;
            *_values.emplace_back(1) = value;
            _kv_slot_lookup.emplace_back(1);
            kv_index = _values._count - 1;
          }
          _slot_kv_index[insert_index] = kv_index;
          _slot_distance_digest[insert_index] = (empty_distance << DISTANCE_SHIFT) | 
                                               (hash(key_hash) & DIGEST_MASK);
          _kv_slot_lookup[kv_index] = insert_index;
          return true;
        } else {
          u32 insert_index = slot_index + swap_distance;
          if (kv_index == NEW_KV) {
            *_keys.emplace_back(1) = key;
            *_values.emplace_back(1) = value;
            _kv_slot_lookup.emplace_back(1);
            kv_index = _values._count - 1;
          }
          u32 swap_kv_index = _slot_kv_index[insert_index];
          K& swap_key = _keys[swap_kv_index];
          V& swap_value = _values[swap_kv_index];
          _slot_kv_index[insert_index] = kv_index;
          _slot_distance_digest[insert_index] = (swap_distance << DISTANCE_SHIFT) | 
                                               (hash(key_hash) & DIGEST_MASK);
          _kv_slot_lookup[kv_index] = insert_index;
          return insert(swap_key, swap_value, swap_kv_index);
        }
      }
      slot_index = (slot_index + 8) & (_capacity - 1);
    }
    if (hash_count < MAX_REHASHES) {
      if (kv_index == NEW_KV) {
        return insert(key, value, kv_index, hash(key_hash), hash_count + 1);
      } else {
        return insert(_keys[kv_index], _values[kv_index], 
                      kv_index, hash(key_hash), hash_count + 1);
      }
    }
    if (kv_index == NEW_KV) {
      *_keys.emplace_back(1) = key;
      *_values.emplace_back(1) = value;
      _kv_slot_lookup.emplace_back(1);
    }
    return rebuild_larger();
  }

  //---
  inline bool remove(const K& key,
                     u32 existing_hash = Types::U32_MAX,
                     u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == Types::U32_MAX) ? hash(key) : existing_hash;
    u32 slot_index = (key_hash & (_capacity - 1));
    for (u32 i = 0; i < _max_probe_length; ++i) {
      slot_index = Math::min((u32)_capacity - 8, slot_index);
      I8 key_digest = I8_SET1(hash(key_hash) & DIGEST_MASK);
      I8 digests = I8_AND(I8_LOADU(&_slot_distance_digest[slot_index]), 
                          I8_SET1(OCCUPIED_AND_DIGEST_MASK));
      u32 digest_mask = I8_MOVEMASK(I8_CMP_EQ(key_digest, digests));
      while (digest_mask) {
        u32 distance = Math::lsb_set(digest_mask) >> 2;
        u32 kv_index = _slot_kv_index[slot_index + distance];
        if (_keys[kv_index] == key) {
          _slot_kv_index[_kv_slot_lookup[_kv_slot_lookup._count - 1]] = kv_index;
          _slot_distance_digest[slot_index + distance] = EMPTY_SLOT;
          _keys.remove(kv_index, 1);
          _values.remove(kv_index, 1);
          _kv_slot_lookup.remove(kv_index, 1);
          return true;
        }
        digest_mask ^= (0xF << (distance << 2));
      }
      slot_index = (slot_index + 8) & (_capacity - 1);
    }
    if (hash_count < MAX_REHASHES) {
      return remove(key, hash(key_hash), hash_count + 1);
    }
    return false;
  }

  //---
  inline bool rebuild_larger()
  {
    _capacity <<= 1;
    _max_probe_length = 1 + (Math::log2(_capacity) >> 2);
    _slot_kv_index = (u32*)realloc_unsafe(_slot_kv_index, 
                                         sizeof(u32) * _capacity);
    _slot_distance_digest = (u32*)realloc_unsafe(_slot_distance_digest, 
                                                sizeof(u32) * _capacity);
    I8 empty_slot = I8_SET1(EMPTY_SLOT);
    for (u32 r = 0; r < (_capacity >> 3); ++r) {
      I8_STORE(&((I8*)_slot_distance_digest)[r], empty_slot);
    }
    for (u32 v = 0; v < _values._count; ++v) {
      insert(_keys[v], _values[v], v);
    }
    return true;
  }
 
  //---
  inline f32 load_factor()
  {
    u32 used_slots = 0;
    for (u32 b = 0; b < _capacity; ++b) {
      used_slots += (_slot_distance_digest[b] != EMPTY_SLOT);
    }
    return ((f32)used_slots / (f32)_capacity);
  }
};
}