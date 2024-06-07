#include "pathlib/profiler/profiler.h"
#include "pathlib/memory/memory.h"

namespace Pathlib::Memory {
//---
namespace _Internal { bool scripting_mode; }


//---
bool clone_memory(void** dst_out,
                  void* src)
{
  ShortStringUnsafe<96> name;
  u64 size;
  if (Pathlib::_Internal::Profiler::get_memory_item(src, name, size)) {
    *dst_out = malloc_unsafe(size, ShortStringUnsafe<96>(name, u8"(clone)")._str);
    memcpy_unsafe(*dst_out, src, size - sizeof(u32));
  } else {
    get_errors().to_log(u8"Failed to copy memory item memory.");
    return false;
  }
  return true;
}
}