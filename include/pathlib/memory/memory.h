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
bool clone_memory(void** dst_out,
                  void* src);

//---
template <typename T,
          typename... Args>
static inline T* call_constructor(void* obj,
                                  Args&&... args)
{
  if constexpr (has_constructor<T, Args...>::value) {
    return new (obj) T(args...);
  } else {
    return nullptr;
  }
}

//---
template <typename T>
static inline void call_destructor(T* obj)
{
  if constexpr (has_destructor<T>::value) {
    obj->~T();
  }
}

//---
namespace _Internal { extern bool scripting_mode; }
}
