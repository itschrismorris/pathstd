/*
  Documentation: https://www.path.blog/docs/memory.html
*/

#pragma once
#include "pathlib/types/types.h"

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

namespace _Internal {

  extern bool scripting_mode;
}
}