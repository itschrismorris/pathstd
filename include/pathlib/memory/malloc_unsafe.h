/*
  Documentation: https://www.path.blog/docs/malloc.html
*/

#pragma once
#include "pathlib/types/types.h"

//---
#define MALLOC(A) ::Pathlib::Memory::malloc(A)
#define REALLOC(A, B) ::Pathlib::Memory::realloc(A, B)
#define FREE(A) ::Pathlib::Memory::free((void**)&A)

namespace Pathlib::Memory {

//---
void* malloc(u64 size,
             const utf8* name = nullptr);
void* realloc(void* ptr,
              u64 size,
              const utf8* name = nullptr);
void free(void** ptr);
}