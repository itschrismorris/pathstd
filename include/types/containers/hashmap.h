/*
  Documentation: https://www.path.blog/docs/hashmap.html
*/

#pragma once
#include "types/types.h"
#include "types/containers/long_vector.h"
#include "types/string/size_of.h"
#include "types/string/long_string.h"

/**/
#define MAX_REHASHES 3
#define EMPTY_BUCKET U32_MAX
#define NEW_INDEX U32_MAX
#define GET_DISTANCE(A) ((A) >> 29)
#define SET_DISTANCE(A) ((A) << 29)
#define GET_INDEX(A) ((A) & 0x1FFFFFFF)
#define CLEAR_INDEX(A) ((A) & 0x70000000)

namespace Pathlib::Containers {

/**/
template <typename K, typename V, u64 RESERVE_CAPACITY = 128LLU>
struct Hashmap
{
  /**/
  struct Bucket
  {
    u32 key_hash;
    u32 distance_index;
  };

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
  u64 max_probe_distance;
  Bucket* buckets;
  Containers::LongVector<KeyValue> values;

  /**/
  Hashmap()
  {
    capacity = Math::round_up_to_pot(RESERVE_CAPACITY);
    buckets = (Bucket*)MALLOC(sizeof(Bucket) * capacity);
    Memory::memset(buckets, (u8)EMPTY_BUCKET, sizeof(Bucket) * capacity);
    max_probe_distance = Math::log2(capacity);
  }

  /**/
  ~Hashmap()
  {
    if (buckets) {
      FREE(buckets);
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
                     u32 index = NEW_INDEX,
                     u32 existing_hash = U32_MAX,
                     u32 hash_count = 0)
  {
    u32 key_hash = (existing_hash == U32_MAX) ? hash(key) : existing_hash;
    u32 distance = 0;
    u32 original_bucket_index = key_hash & (capacity - 1);
    for (u32 p = 0; p < max_probe_distance; ++p) {
      u32 bucket_index = (original_bucket_index + p) & (capacity - 1);
      Bucket* bucket = &buckets[bucket_index];
      if (bucket->distance_index == EMPTY_BUCKET) {
        if (index == NEW_INDEX) {
          *values.emplace_back(1) = KeyValue(key, value);
          index = values.count - 1;
        }
        bucket->key_hash = key_hash;
        bucket->distance_index = SET_DISTANCE(distance) | index;
        values[index].bucket_index = bucket_index;
        return true;
      } else if (bucket->key_hash == key_hash) {
        if (values[GET_INDEX(bucket->distance_index)].key == key) {
          return false;
        }
      }
      if (distance > GET_DISTANCE(bucket->distance_index)) {
        if (index == NEW_INDEX) {
          *values.emplace_back(1) = KeyValue(key, value);
          index = values.count - 1;
        }
        Bucket swap_bucket = *bucket;
        bucket->key_hash = key_hash;
        bucket->distance_index = SET_DISTANCE(distance) | index;
        values[index].bucket_index = bucket_index;
        index = GET_INDEX(swap_bucket.distance_index);
        distance = GET_DISTANCE(swap_bucket.distance_index);
        key_hash = swap_bucket.key_hash;
      }
      ++distance;
    }
    if (hash_count < MAX_REHASHES) {
      if (index == NEW_INDEX) {
        return insert(key, value, index, hash(key_hash), hash_count + 1);
      } else {
        return insert(values[index].key, values[index].value, index, hash(key_hash), hash_count + 1);
      }
    }
    if (index == NEW_INDEX) {
      *values.emplace_back(1) = KeyValue(key, value);
    }
    return rebuild_larger();
  }

  /**/
  inline bool remove(const K& key,
                     u32 existing_hash = U32_MAX,
                     u32 hash_count = 0)
  {
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
    }
    return false;
  }

  /**/
  inline bool rebuild_larger()
  {
    console.write(u8"Distance: ", max_probe_distance);
    console.write(u8"Capacity (bytes): ", (capacity * sizeof(Bucket)) + (values.capacity * sizeof(KeyValue)));
    console.write(load_factor());
    capacity <<= 1;
    max_probe_distance = Math::log2(capacity);
    buckets = (Bucket*)REALLOC(buckets, sizeof(Bucket) * capacity);
    Memory::memset(buckets, (u8)EMPTY_BUCKET, sizeof(Bucket) * capacity);
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
      if (buckets[b].distance_index != EMPTY_BUCKET) {
        ++used_buckets;
      }
    }
    return ((f32)used_buckets / (f32)capacity);
  }
};
}