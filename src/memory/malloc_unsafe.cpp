#include "pathlib/memory/malloc.h"
#include "pathlib/types/containers/safe_ptr.h"
#include "../src/third_party/mimalloc/mimalloc.h"

namespace Pathlib::Memory {

//---
Containers::SafePtr<u8> malloc(u64 size,
                               const utf8* name)
{
  Containers::SafePtr<u8> ptr((u8*)mi_malloc_aligned(size, 128), size);
  return ptr;
}

//---
Containers::SafePtr<u8> realloc(Containers::SafePtr<u8> ptr,
                                u64 size,
                                const utf8* name)
{
  void* p = mi_realloc_aligned(ptr, size, 128);
  return new_ptr;
}

//---
void free(void** ptr)
{
  mi_free_aligned(*ptr, 128);
}
}