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
void* p_alloc(u64 size,
              const char* name)
{
  void* ptr = mi_malloc_aligned(size, 32);
  return ptr;
}

/**/
void* p_realloc(void* ptr,
                u64 size,
                const char* name)
{
  void* new_ptr = mi_realloc_aligned(ptr, size, 32);
  return new_ptr;
}

/**/
void p_free(void** ptr)
{
  mi_free_aligned(ptr, 32);
}
}