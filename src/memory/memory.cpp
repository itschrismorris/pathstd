#include "memory/memory.h"

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
              const char* name)
{
  void* ptr = mi_malloc_aligned(size, 128);
  return ptr;
}

/**/
void* realloc(void* ptr,
                u64 size,
                const char* name)
{
  void* new_ptr = mi_realloc_aligned(ptr, size, 128);
  return new_ptr;
}

/**/
void free(void** ptr)
{
  mi_free_aligned(ptr, 128);
}
}