/*
  Documentation: https://www.path.blog/docs/profile.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/containers/pools_unsafe.h"
#include "pathlib/types/string/short_string_unsafe.h"

namespace Pathlib::_Internal {

//---
struct Profile
{
  //---
  struct MemoryItem
  {
    ShortStringUnsafe<32> name;
    u32 pool_id;
    u64 size;
  };

  //---
  bool initiate();
  void shutdown();

  //---
  PoolsUnsafe<MemoryItem, 1024, 8> memory_items;
};
}

//---
namespace Pathlib { extern _Internal::Profile profile; }