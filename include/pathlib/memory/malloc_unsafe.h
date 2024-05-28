/*
  Documentation: https://www.path.blog/docs/malloc.html
*/

#pragma once
#include "pathlib/types/types.h"

//---
#define MALLOC(A) ::Pathlib::Memory::malloc_unsafe(A)
#define REALLOC(A, B) ::Pathlib::Memory::realloc_unsafe(A, B)
#define FREE(A) ::Pathlib::Memory::free_unsafe((void**)&A)

namespace Pathlib::Memory {

//---
void* malloc_unsafe(u64 size,
                    const utf8* name = nullptr);
void* realloc_unsafe(void* ptr,
                     u64 size,
                     const utf8* name = nullptr);
void free_unsafe(void** ptr);
}