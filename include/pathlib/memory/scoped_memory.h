/*
  Documentation: https://www.path.blog/docs/memset.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memory.h"

namespace Pathlib {

//---
template <typename T>
struct ScopedMemory
{
  //---
  T* _ptr;
  u64 _count;

  //---
  ScopedMemory(u64 count)
  {
    _ptr = (T*)get_memory().scoped_arena.push(sizeof(T) * count);
    _count = count;
  }

  //---
  ~ScopedMemory()
  {
    get_memory().scoped_arena.pop(sizeof(T) * _count);
  }

  //---
  inline T* get() const { return _ptr; }

  //---
  inline T* operator()() const { return _ptr; }
};
}