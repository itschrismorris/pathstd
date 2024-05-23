#include "pathlib/memory/malloc.h"
#include "mimalloc/mimalloc.h"

namespace Pathlib::Memory {

//---
void* malloc(u64 size,
             const utf8* name)
{
  void* ptr = mi_malloc_aligned(size, 128);
  return ptr;
}

//---
void* realloc(void* ptr,
              u64 size,
              const utf8* name)
{
  void* new_ptr = mi_realloc_aligned(ptr, size, 128);
  return new_ptr;
}

//---
void free(void** ptr)
{
  mi_free_aligned(*ptr, 128);
}
}