#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/profiler/profiler.h"
#include "pathlib/containers/pool_unsafe.h"
#include "../src/third_party/mimalloc/mimalloc.h"

#define NOT_TRACKED 0xFFFFFFFF

namespace Pathlib {

//---
void* malloc_unsafe(u64 size,
                    const utf8* name)
{
  u8* ptr = (u8*)mi_malloc_aligned(size + 64, 64);
  if (EXPECT(name != nullptr)) {
    u64 copy_size = Math::min(63LLU, StringUtilities::length_of(name));
    memcpy_unsafe(ptr, name, copy_size);
    ptr[copy_size] = '\0';
  } else {
    const utf8* unnamed = u8"Unnamed";
    memcpy_unsafe(ptr, unnamed, 7);
    ptr[7] = '\0';
  }
  return (ptr + 64);
}

//---
void* realloc_unsafe(void* ptr,
                     u64 size)
{
  return (u8*)mi_realloc_aligned((u8*)ptr - 64, size + 64, 64) + 64;
}

//---
void free_unsafe(void** ptr)
{
  mi_free_aligned((u8*)(*ptr) - 64, 64);
  *ptr = nullptr;
}
}