/*
  Documentation: https://www.path.blog/docs/hashmap.html
*/

#pragma once
#include "types/types.h"
#include "types/containers/long_vector.h"
#include "types/string/size_of.h"
#include "types/string/long_string.h"

/**/
#define MAX_REHASHES 0
#define MAX_PROBE_DISTANCE_BITS 3
#define DISTANCE_SHIFT (32 - MAX_PROBE_DISTANCE_BITS)
#define MAX_PROBE_DISTANCE (0x1 << MAX_PROBE_DISTANCE_BITS)
#define DIGEST_MASK (0xFFFFFFFF >> (MAX_PROBE_DISTANCE_BITS + 1))
#define EMPTY_BUCKET (0xFFFFFFFF >> MAX_PROBE_DISTANCE_BITS)
#define NEW_INDEX U32_MAX
#define GET_DISTANCE(A) ((A) >> 29)
#define SET_DISTANCE(A) ((A) << 29)
#define GET_INDEX(A) ((A) & 0x1FFFFFFF)
#define CLEAR_INDEX(A) ((A) & 0x70000000)

namespace Pathlib::Containers {

/**/
template <typename K, typename V>
struct Hashmap
{

  /**/
  struct KeyValue
  {
    u32 bucket_index;
    K key;
    V value;

    /**/
    KeyValue(const K& _key, 
             const V& _value)
    {
      key = _key;
      value = _value;
    }
  };

  /**/
  u64 capacity;
  u32* bucket_value_index;
  u32* bucket_distance_digest;
  Containers::LongVector<KeyValue, 128> values;

  /**/
  Hashmap()
  {
    capacity = 128;
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
    if constexpr (IS_INTEGRAL(K) || IS_FLOAT(K)) {
      return Math::hash(key);
    } else if constexpr (IS_SHORT_STRING(K)) {
      return key.hash();
    } else if constexpr (IS_LONG_STRING(K)) {
      return key.hash();
    } else if constexpr (SAME_TYPE(K, const utf8*)) {
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
    /*
    u32 key_hash = (existing_hash == U32_MAX) ? hash(key) : existing_hash;
    for (u32 p = 0; p < max_probe_distance; ++p) {
      u32 bucket_index = (key_hash + p) & (capacity - 1);
      Bucket* bucket = &buckets[bucket_index];
      if (bucket->distance_index != EMPTY_BUCKET) {
        if (bucket->key_hash == key_hash) {
          u32 value_index = GET_INDEX(bucket->distance_index);
          if (values[value_index].key == key) {
            return &values[value_index].value;
          }
        }
      } else {
        if (hash_count < MAX_REHASHES) {
          return find(key, hash(key_hash), hash_count + 1);
        }
        return nullptr;
      }
    }
    if (hash_count < MAX_REHASHES) {
      return find(key, hash(key_hash), hash_count + 1);
    }*/
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
                     u32 value_index = NEW_INDEX,
                     u32 existing_bucket = U32_MAX,
                     u32 existing_hash = U32_MAX,
                     u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == U32_MAX) ? hash(key) : existing_hash;
    u32 bucket_index = (existing_bucket == U32_MAX) ? (key_hash & (capacity - 1)) : existing_bucket;
    bucket_index = Math::min((u32)capacity - MAX_PROBE_DISTANCE, bucket_index);
    //bucket_index = Math::previous_multiple_of<u32, 8>(bucket_index);

    // Now check 8 values for empty bucket, and 8 values for less distance than ours.
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
        if (value_index == NEW_INDEX) {
          *values.emplace_back(1) = KeyValue(key, value);
          value_index = values.count - 1;
        }
        bucket_value_index[insert_index] = value_index;
        bucket_distance_digest[insert_index] = SET_DISTANCE(empty_distance) | (key_hash & DIGEST_MASK);
        values[value_index].bucket_index = insert_index;
        return true;
      } else {
        u32 insert_index = bucket_index + distance_distance;
        if (value_index == NEW_INDEX) {
          *values.emplace_back(1) = KeyValue(key, value);
          value_index = values.count - 1;
        }
        u32 swap_value_index = bucket_value_index[insert_index];
        KeyValue* swap = &values[swap_value_index];
        bucket_value_index[insert_index] = value_index;
        bucket_distance_digest[insert_index] = SET_DISTANCE(distance_distance) | (key_hash & DIGEST_MASK);
        values[value_index].bucket_index = insert_index;
        return insert(swap->key, swap->value, swap_value_index, insert_index);
      }
    }
    if (hash_count < MAX_REHASHES) {
      if (value_index == NEW_INDEX) {
        return insert(key, value, value_index, U32_MAX, hash(key_hash), hash_count + 1);
      } else {
        return insert(values[value_index].key, values[value_index].value, value_index, U32_MAX, hash(key_hash), hash_count + 1);
      }
    }
    if (value_index == NEW_INDEX) {
      *values.emplace_back(1) = KeyValue(key, value);
    }
    return rebuild_larger();
  }

  /**/
  inline bool remove(const K& key,
                     u32 existing_hash = U32_MAX,
                     u32 hash_count = 0)
  {
    /*
    u32 key_hash = (existing_hash == U32_MAX) ? hash(key) : existing_hash;
    for (u32 p = 0; p < max_probe_distance; ++p) {
      u32 bucket_index = (key_hash + p) & (capacity - 1);
      Bucket* bucket = &buckets[bucket_index];
      if (bucket->distance_index == EMPTY_BUCKET) {
        return true;
      }
      if (bucket->key_hash == key_hash) {
        u32 value_index = GET_INDEX(bucket->distance_index);
        if (values[value_index].key == key) {
          values.remove(value_index, 1);
          Bucket* moved_bucket = &buckets[values[value_index].bucket_index];
          moved_bucket->distance_index = CLEAR_INDEX(moved_bucket->distance_index) | value_index;
          bucket->distance_index = EMPTY_BUCKET;
          for (u32 shift = 1; shift <= values.count; ++shift) {
            u32 next_bucket_index = (bucket_index + shift) & (capacity - 1);
            Bucket* next_bucket = &buckets[next_bucket_index];
            u32 next_bucket_distance = GET_DISTANCE(next_bucket->distance_index);
            if ((next_bucket->distance_index == EMPTY_BUCKET) ||
                (next_bucket_distance <= shift)) {
              bucket->distance_index = EMPTY_BUCKET;
              break;
            }
            u32 next_bucket_value_index = GET_INDEX(next_bucket->distance_index);
            bucket->distance_index = SET_DISTANCE(next_bucket_distance - 1) | next_bucket_value_index;
            bucket->key_hash = next_bucket->key_hash;
            values[next_bucket_value_index].bucket_index = bucket - buckets;
            bucket = next_bucket;
          }
          return true;
        }
      }
      if (hash_count < MAX_REHASHES) {
        return remove(key, hash(key_hash), hash_count + 1);
      }
    }*/
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
      insert(values[v].key, values[v].value, v);
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