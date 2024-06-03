/*
  Documentation: https://www.path.blog/docs/malloc.html
*/

#pragma once
#include "pathlib/types/types.h"

namespace Pathlib {

//---
void* malloc_unsafe(u64 size,
                    const utf8* name);
void* realloc_unsafe(void* ptr,
                     u64 size);
void free_unsafe(void** ptr);
}
