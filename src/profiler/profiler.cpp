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
Profiler::Profiler() : _memory_items(nullptr)
{
  #ifdef _DEBUG
    mi_option_enable(mi_option_verbose);
  #endif
}

//---
bool Profiler::get_memory_item(void* ptr,
                               ShortStringUnsafe<96>& name_out,
                               u64& size_out)
{
  u64 memory_size = mi_usable_size(ptr);
  u32 pool_id = *(u32*)(&((u8*)ptr)[memory_size - sizeof(u32)]);
  _Internal::Profiler::MemoryItem* memory_item = get_profiler()._memory_items[pool_id];
  if (memory_item) {
    name_out = memory_item->_name;
    size_out = memory_item->_size;
  } else {
    get_errors().to_log(u8"Failed to retrieve memory item; does not exist in '_memory_items'.");
    return false;
  }
  return true;
}
}