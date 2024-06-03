/*
  Documentation: https://www.path.blog/docs/profile.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/containers/pools_unsafe.h"
#include "pathlib/types/string/short_string_unsafe.h"

namespace Pathlib::_Internal {

//---
struct Profiler
{
  //---
  struct MemoryItem
  {
    ShortStringUnsafe<96> name;
    u32 pool_id;
    u64 size;
  };

  //---
  PoolsUnsafe<MemoryItem, 1024, 8> memory_items;

  //---
  Profiler();
  ~Profiler() {}

  //---
  static ShortStringUnsafe<96> get_memory_item_name(void* ptr);
};
}

//---
namespace Pathlib { _Internal::Profiler& get_profiler(); }