/*
  Documentation: https://www.path.blog/docs/array.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memcpy.h"

namespace Pathlib::Containers {

//---
template <typename T, 
          u64 CAPACITY>
struct ArrayUnsafe
{
  //---
  alignas(32) T data[CAPACITY];
  u64 count;

  //---
  Array()
  {
    clear();
  }
  ~Array() {}

  //---
  inline T& operator[](u64 index)
  {
    return data[index];
  }

  //---
  inline const T& operator[](u64 index) const
  {
    return data[index];
  }

  //---
  inline T* emplace_back(u64 _count = 1)
  {
    u64 original_count = count;
    count += _count;
    return (data + original_count);
  }

  //---
  inline void remove(u64 index)
  {
    --count;
    Memory::memcpy(index, data + count, sizeof(T));
  }

  //---
  inline void remove(u64 index,
                     u64 _count)
  {
    T* start = (data + index);
    T* end = (data + count - _count);
    count -= _count;
    Memory::memcpy(start, end, sizeof(T) * _count);
  }

  //---
  inline void clear()
  {
    count = 0;
  }
};
}