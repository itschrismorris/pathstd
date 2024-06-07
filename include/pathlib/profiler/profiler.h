/*
  Documentation: https://www.path.blog/docs/profile.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/containers/pools_unsafe.h"
#include "pathlib/string/short_string_unsafe.h"

namespace Pathlib::_Internal {

//---
struct Profiler
{
  //---
  struct MemoryItem
  {
    ShortStringUnsafe<96> _name;
    u32 _pool_id;
    u64 _size;
  };

  //---
  PoolsUnsafe<MemoryItem, 1024, 8> _memory_items;

  //---
  Profiler();
  ~Profiler() {}

  //---
  static bool get_memory_item(void* ptr,
                              ShortStringUnsafe<96>& name_out,
                              u64& size_out);
};
}

//---
namespace Pathlib { _Internal::Profiler& get_profiler(); }