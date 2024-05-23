/*
  Documentation: https://www.path.blog/docs/hashmap.html
*/

#pragma once
#include "types/types.h"
#include "types/containers/long_vector.h"
#include "types/string/size_of.h"
#include "types/string/long_string.h"

/**/
#define MAX_REHASHES 1
#define MAX_PROBE_CYCLES 2
#define MAX_PROBE_DISTANCE_BITS 3
#define DISTANCE_SHIFT (32 - MAX_PROBE_DISTANCE_BITS)
#define MAX_PROBE_DISTANCE (0x1 << MAX_PROBE_DISTANCE_BITS)
#define DIGEST_MASK (0xFFFFFFFF >> (MAX_PROBE_DISTANCE_BITS + 1))
#define DIGEST_MASK_READ (0xFFFFFFFF >> MAX_PROBE_DISTANCE_BITS)
#define EMPTY_BUCKET (0xFFFFFFFF >> MAX_PROBE_DISTANCE_BITS)
#define NEW_VALUE_INDEX U32_MAX
#define SET_DISTANCE(A) ((A) << (DISTANCE_SHIFT))

namespace Pathlib::Containers {

/**/
template <typename K, typename V>
struct Hashmap
{
  /**/
  u64 capacity;
  u32* bucket_value_index;
  u32* bucket_distance_digest;
  Containers::LongVector<K, 64> keys;
  Containers::LongVector<V, 64> values;
  Containers::LongVector<u32, 64> bucket_indexes;

  /**/
  Hashmap()
  {
    capacity = 64;
    bucket_value_index = (u32*)MALLOC(sizeof(u32) * capacity);
    bucket_distance_digest = (u32*)MALLOC(sizeof(u32) * capacity);
    I8 empty_bucket = I8_SET1(EMPTY_BUCKET);
    for (u32 r = 0; r < (capacity >> 3); ++r) {
      I8_STORE(&((I8*)bucket_distance_digest)[r], empty_bucket);
    }
  }

  /**/
  ~Hashmap()
  {
    if (bucket_value_index) {
      FREE(bucket_value_index);
    }
    if (bucket_distance_digest) {
      FREE(bucket_distance_digest);
    }
  }
  
  /**/
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
      return String::LongString<>::hash(key);
    } else {
      static_assert(false, "Unsupported type used for hashmap key.");
    }
  }

  /**/
  inline V* find(const K& key,
                 u32 existing_hash = U32_MAX,
                 u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == U32_MAX) ? hash(key) : existing_hash;
    u32 bucket_index = (key_hash & (capacity - 1));

    for (u32 i = 0; i < MAX_PROBE_CYCLES; ++i) {
      bucket_index = Math::min((u32)capacity - MAX_PROBE_DISTANCE, bucket_index);
      I8 key_digest = I8_SET1(hash(key_hash) & DIGEST_MASK);
      I8 digests = I8_AND(I8_LOADU(&bucket_distance_digest[bucket_index]), I8_SET1(DIGEST_MASK_READ));
      u32 digest_mask = I8_MOVEMASK(I8_CMP_EQ(key_digest, digests));
      while (digest_mask) {
        u32 distance = Math::lsb_set(digest_mask) >> 2;
        u32 value_index = bucket_value_index[bucket_index + distance];
        if (keys[value_index] == key) {
          return &values[value_index];
        }
        digest_mask ^= (0xF << (distance << 2));
      }
      bucket_index += 8;
    }
    if (hash_count < MAX_REHASHES) {
      return find(key, hash(key_hash), hash_count + 1);
    }
    return nullptr;
  }

  /**/
  V* operator [](const K& key)
  {
    return find(key);
  }

  /**/
  inline bool insert(const K& key,
                     const V& value,
                     u32 value_index = NEW_VALUE_INDEX,
                     u32 existing_bucket = U32_MAX,
                     u32 existing_hash = U32_MAX,
                     u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == U32_MAX) ? hash(key) : existing_hash;
    u32 bucket_index = (existing_bucket == U32_MAX) ? (key_hash & (capacity - 1)) : existing_bucket;
    for (u32 i = 0; i < MAX_PROBE_CYCLES; ++i) {
      bucket_index = Math::min((u32)capacity - MAX_PROBE_DISTANCE, bucket_index);
      I8 probe = I8_LOADU(&bucket_distance_digest[bucket_index]);
      I8 empty_bucket = I8_SET1(EMPTY_BUCKET);
      I8 distance_check = I8_SET(0, 1, 2, 3, 4, 5, 6, 7);
      I8 probe_distances = I8_SHIFTR(probe, DISTANCE_SHIFT);
      u32 empty_mask = I8_MOVEMASK(I8_CMP_EQ(probe, empty_bucket));
      u32 distance_mask = I8_MOVEMASK(I8_CMP_GT(probe_distances, distance_check));
      if (empty_mask || distance_mask) {
        u32 empty_distance = Math::lsb_set(empty_mask) >> 2;
        u32 distance_distance = Math::lsb_set(distance_mask) >> 2;
        if (empty_distance <= distance_distance) {
          u32 insert_index = bucket_index + empty_distance;
          if (value_index == NEW_VALUE_INDEX) {
            *keys.emplace_back(1) = key;
            *values.emplace_back(1) = value;
            bucket_indexes.emplace_back(1);
            value_index = values.count - 1;
          }
          bucket_value_index[insert_index] = value_index;
          bucket_distance_digest[insert_index] = SET_DISTANCE(empty_distance) | (hash(key_hash) & DIGEST_MASK);
          bucket_indexes[value_index] = insert_index;
          return true;
        } else {
          u32 insert_index = bucket_index + distance_distance;
          if (value_index == NEW_VALUE_INDEX) {
            *keys.emplace_back(1) = key;
            *values.emplace_back(1) = value;
            bucket_indexes.emplace_back(1);
            value_index = values.count - 1;
          }
          u32 swap_value_index = bucket_value_index[insert_index];
          K& swap_key = keys[swap_value_index];
          V& swap_value = values[swap_value_index];
          bucket_value_index[insert_index] = value_index;
          bucket_distance_digest[insert_index] = SET_DISTANCE(distance_distance) | (hash(key_hash) & DIGEST_MASK);
          bucket_indexes[value_index] = insert_index;
          return insert(swap_key, swap_value, swap_value_index);
        }
      }
      bucket_index += 8;
    }
    if (hash_count < MAX_REHASHES) {
      if (value_index == NEW_VALUE_INDEX) {
        return insert(key, value, value_index, U32_MAX, hash(key_hash), hash_count + 1);
      } else {
        return insert(keys[value_index], values[value_index], value_index, U32_MAX, hash(key_hash), hash_count + 1);
      }
    }
    if (value_index == NEW_VALUE_INDEX) {
      *keys.emplace_back(1) = key;
      *values.emplace_back(1) = value;
      bucket_indexes.emplace_back(1);
    }
    return rebuild_larger();
  }

  /**/
  inline bool remove(const K& key,
                     u32 existing_hash = U32_MAX,
                     u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == U32_MAX) ? hash(key) : existing_hash;
    u32 bucket_index = (key_hash & (capacity - 1));
    for (u32 i = 0; i < MAX_PROBE_CYCLES; ++i) {
      bucket_index = Math::min((u32)capacity - MAX_PROBE_DISTANCE, bucket_index);
      I8 key_digest = I8_SET1(hash(key_hash) & DIGEST_MASK);
      I8 digests = I8_AND(I8_LOADU(&bucket_distance_digest[bucket_index]), I8_SET1(DIGEST_MASK_READ));
      u32 digest_mask = I8_MOVEMASK(I8_CMP_EQ(key_digest, digests));
      while (digest_mask) {
        u32 distance = Math::lsb_set(digest_mask) >> 2;
        u32 value_index = bucket_value_index[bucket_index + distance];
        if (keys[value_index] == key) {
          bucket_value_index[bucket_indexes[bucket_indexes.count - 1]] = value_index;
          bucket_distance_digest[bucket_index + distance] = EMPTY_BUCKET;
          keys.remove(value_index, 1);
          values.remove(value_index, 1);
          bucket_indexes.remove(value_index, 1);
          return true;
        }
        digest_mask ^= (0xF << (distance << 2));
      }
      bucket_index += 8;
    }
    if (hash_count < MAX_REHASHES) {
      return remove(key, hash(key_hash), hash_count + 1);
    }
    return false;
  }

  /**/
  inline bool rebuild_larger()
  {
    console.write(load_factor());
    capacity <<= 1;
    bucket_value_index = (u32*)REALLOC(bucket_value_index, sizeof(u32) * capacity);
    bucket_distance_digest = (u32*)REALLOC(bucket_distance_digest, sizeof(u32) * capacity);
    I8 empty_bucket = I8_SET1(EMPTY_BUCKET);
    for (u32 r = 0; r < (capacity >> 3); ++r) {
      I8_STORE(&((I8*)bucket_distance_digest)[r], empty_bucket);
    }
    for (u32 v = 0; v < values.count; ++v) {
      insert(keys[v], values[v], v);
    }
    return true;
  }
 
  /**/
  inline f32 load_factor()
  {
    u32 used_buckets = 0;
    for (u32 b = 0; b < capacity; ++b) {
      used_buckets += (bucket_distance_digest[b] != EMPTY_BUCKET);
    }
    return ((f32)used_buckets / (f32)capacity);
  }
};
}