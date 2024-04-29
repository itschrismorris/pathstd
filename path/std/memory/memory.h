/* 'std/memory/memory.h' 

  + Uses Microsoft's mimalloc library: https://github.com/microsoft/mimalloc
  + All allocations are 32-byte aligned.
  + Names are fed to memory profiler, but are optional.

    Path game engine: https://www.path.blog
*/

#pragma once
#include "mimalloc/include/mimalloc.h"
#include "std/types.h"

#define PAGE_SIZE 0x1000

namespace Path::Std::Memory {

  /**/
  void initiate();
  void shutdown();
  void* p_alloc(u64 size,
                const char* name = nullptr);
  void* p_realloc(void* ptr,
                  u64 size,
                  const char* name = nullptr);
  void p_free(void** ptr);
}