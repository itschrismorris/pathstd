/*
  Documentation: https://www.path.blog/docs/hashmap.html
*/

#pragma once
#include "types/types.h"
#include "types/containers/long_vector.h"
#include "types/string/size_of.h"
#include "types/string/long_string.h"

/**/
#define EMPTY_BUCKET U32_MAX
#define NEW_VALUE U32_MAX
#define GET_DISTANCE(A) (A >> 29)
#define SET_DISTANCE(A) (A << 29)
#define GET_INDEX(A) (A & 0x1FFFFFFF)
#define MAX_PROBE_DISTANCE 8

namespace Pathlib::Containers {

/**/
template <typename K, typename V, u64 RESERVE_CAPACITY = 64LLU>
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
  Bucket* buckets;
  Containers::LongVector<KeyValue> values;

  /**/
  Hashmap()
  {
    capacity = Math::round_up_to_pot(RESERVE_CAPACITY);
    buckets = (Bucket*)MALLOC(sizeof(Bucket) * capacity);
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
      static_assert(false, "Unsupported type used for hashmap Key.");
    }
  }

  /**/
  V* operator [](const K& key)
  {
    u32 key_hash = hash(key);
    u32 index = key_hash & (capacity - 1);
    for (u32 p = 0; p < MAX_PROBE_DISTANCE; ++p) {
      u32 bucket_index = (index + p) & (capacity - 1);
      Bucket* bucket = &buckets[bucket_index];
      if (bucket->distance_index != EMPTY_BUCKET) {
        if (bucket->key_hash == key_hash) {
          u32 value_index = GET_INDEX(bucket->distance_index);
          if (values[value_index].key == key) {
            return &values[value_index].value;
          }
        }
      } else {
        return nullptr;
      }
    }
    return nullptr;
  }

  /**/
  inline bool insert(const K& key,
                     const V& value,
                     u32 existing_index = NEW_VALUE)
  {
    u32 key_hash = hash(key);
    u32 index = key_hash & (capacity - 1);
    u32 distance = 0;
    for (u32 p = 0; p < MAX_PROBE_DISTANCE; ++p) {
      u32 bucket_index = (index + p) & (capacity - 1);
      Bucket* bucket = &buckets[bucket_index];
      if (bucket->distance_index == EMPTY_BUCKET) {
        if (existing_index == NEW_VALUE) {
          *values.emplace_back(1) = KeyValue(key, value);
          existing_index = values.count - 1;
        }
        bucket->key_hash = key_hash;
        bucket->distance_index = SET_DISTANCE(distance) | existing_index;
        values[existing_index].bucket_index = bucket_index;
        return true;
      } else if (bucket->key_hash == key_hash) {
        if (values[GET_INDEX(bucket->distance_index)].key == key) {
          return false;
        }
      }
      if (distance > GET_DISTANCE(bucket->distance_index)) {
        if (existing_index == NEW_VALUE) {
          *values.emplace_back(1) = KeyValue(key, value);
          existing_index = values.count - 1;
        }
        Bucket swap_bucket = *bucket;
        bucket->key_hash = key_hash;
        bucket->distance_index = SET_DISTANCE(distance) | existing_index;
        values[existing_index].bucket_index = bucket_index;
        existing_index = GET_INDEX(swap_bucket.distance_index);
        distance = GET_DISTANCE(swap_bucket.distance_index);
        key_hash = swap_bucket.key_hash;
      }
      ++distance;
    }
    if (existing_index == NEW_VALUE) {
      KeyValue* kv = values.emplace_back(1);
      kv->key = key;
      kv->value = value;
    }
    return false;
  }
};
}