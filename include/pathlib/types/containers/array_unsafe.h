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
  ArrayUnsafe()
  {
    clear();
  }
  ~ArrayUnsafe() {}

  //---
  template <typename... Args>
  void initializer_list(u64 index, 
                        T value, 
                        Args... args)
  {
    count = index + 1;
    data[index] = value;
    initializer_list(index + 1, args...);
  }

  //---
  void initializer_list(u64 index) {}

  //---
  template <typename... Args>
  ArrayUnsafe(Args... args)
  {
    static_assert(sizeof...(Args) <= CAPACITY, "Initializer list exceeds Array capacity.");
    initializer_list(0, args...);
  }

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
    Memory::memcpy_unsafe(index, data + count, sizeof(T));
  }

  //---
  inline void remove(u64 index,
                     u64 _count)
  {
    T* start = (data + index);
    T* end = (data + count - _count);
    count -= _count;
    Memory::memcpy_unsafe(start, end, sizeof(T) * _count);
  }

  //---
  inline void clear()
  {
    count = 0;
  }
};
}