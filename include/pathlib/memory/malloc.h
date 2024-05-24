/*
  Documentation: https://www.path.blog/docs/malloc.html
*/

#pragma once
#include "pathlib/types/types.h"

//---
#define MALLOC(A) ::Pathlib::Memory::malloc(A)
#define REALLOC(A, B) ::Pathlib::Memory::realloc(A, B)
#define FREE(A) ::Pathlib::Memory::free((void**)&A)

//---
inline void* operator new(size_t, void* ptr) 
{
  return ptr;
}

namespace Pathlib::Memory {

//---
static constexpr u32 PAGE_SIZE = 0x1000;
static constexpr u32 KILOBYTE = 1024;
static constexpr u32 MEGABYTE = (1024 * 1024);
static constexpr u32 GIGABYTE = (1024 * 1024 * 1024);

//---
void* malloc(u64 size,
             const utf8* name = nullptr);
void* realloc(void* ptr,
              u64 size,
              const utf8* name = nullptr);
void free(void** ptr);

//---
template <typename T, 
          typename... Args>
static inline T* call_constructor(void* obj, 
                                  Args&&... args)
{
  return new (obj) T(args...);
}

//---
template <typename T>
static inline void call_destructor(T* obj)
{
  obj->~T();
}
}