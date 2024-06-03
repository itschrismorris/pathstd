#include "pathlib/pathlib.h"
#include "../src/third_party/mimalloc/mimalloc.h"

namespace Pathlib {
//---
_Internal::Profiler& get_profiler() 
{
  static _Internal::Profiler profiler;
  return profiler;
}
}

namespace Pathlib::_Internal {

//---
Profiler::Profiler() : memory_items(nullptr)
{
  #ifdef _DEBUG
    mi_option_enable(mi_option_verbose);
  #endif
}

//---
ShortStringUnsafe<96> Profiler::get_memory_item_name(void* ptr)
{
  u64 memory_size = mi_usable_size(ptr);
  u32 pool_id = *(u32*)(&((u8*)ptr)[memory_size - sizeof(u32)]);
  _Internal::Profiler::MemoryItem* memory_item = get_profiler().memory_items[pool_id];
  return memory_item->name;
}
}