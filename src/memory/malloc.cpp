#include "string/short_string.h"
#include "third_party/mimalloc/mimalloc.h"
#include "memory/malloc.h"

namespace Pathlib::Memory {

/**/
void initiate()
{
  #ifdef _DEBUG
    mi_option_enable(mi_option_verbose);
  #endif
}

/**/
void shutdown()
{
}

/**/
void* malloc(u64 size,
             const utf8* name)
{
  void* ptr = mi_malloc_aligned(size, 128);
  return ptr;
}

/**/
void* realloc(void* ptr,
              u64 size,
              const utf8* name)
{
  void* new_ptr = mi_realloc_aligned(ptr, size, 128);
  return new_ptr;
}

/**/
void free(void** ptr)
{
  mi_free_aligned(*ptr, 128);
}
}