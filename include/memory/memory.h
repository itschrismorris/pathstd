/* 'std/memory/memory.h' 

  + Uses Microsoft's mimalloc library: https://github.com/microsoft/mimalloc
  + All allocations are 32-byte aligned.
  + Names are fed to memory profiler, but are optional.

    Path game engine: https://www.path.blog
*/

#pragma once
#include "third_party/mimalloc/mimalloc.h"
#include "types.h"

#define PAGE_SIZE 0x1000
#define KILOBYTE (1024)
#define MEGABYTE (1024 * 1024)
#define GIGABYTE (1024 * 1024 * 1024)

namespace Pathlib::Memory {

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