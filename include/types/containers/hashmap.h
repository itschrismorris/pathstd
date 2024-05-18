/* 'std/containers/hashmap.h'

  + Open-addressed, relocating (Robinhood), not thread-safe, no duplicate keys.
  + Key/Value pairs are stored contigously for fast iterating.

    Path: https://www.path.blog
*/

#pragma once
#include "../types.h"

namespace Pathlib::Containers {

/**/
template <typename K, typename V, u64 C = 1024>
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
  };

  /**/
  char name[64];
  u64 capacity;
  Bucket* buckets;

  /**/
  Hashmap() : capacity(0), buckets(nullptr) { }
  ~Hashmap() { }
  bool initiate() { }
  void shutdown() { }
};
}