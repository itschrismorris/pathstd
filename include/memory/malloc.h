/*
  Documentation: https://www.path.blog/docs/malloc.html
*/
#pragma once
#include "types/types.h"

#define PAGE_SIZE 0x1000
#define KILOBYTE (1024)
#define MEGABYTE (1024 * 1024)
#define GIGABYTE (1024 * 1024 * 1024)
#define MALLOC(A) ::Pathlib::Memory::malloc(A)
#define REALLOC(A, B) ::Pathlib::Memory::realloc(A, B)
#define FREE(A) ::Pathlib::Memory::free((void**)&A)

namespace Pathlib::Memory {

/**/
void* malloc(u64 size,
             const utf8* name = nullptr);
void* realloc(void* ptr,
              u64 size,
              const utf8* name = nullptr);
void free(void** ptr);
}