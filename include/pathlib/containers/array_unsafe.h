/*
  Documentation: https://www.path.blog/docs/array.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memcpy.h"

namespace Pathlib {

//---
template <typename T, 
          u64 CAPACITY>
struct ArrayUnsafe
{
  //---
  alignas(32) T _data[CAPACITY];
  u64 _count;

  //---
  DISALLOW_COPY(ArrayUnsafe);

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
    _count = index + 1;
    _data[index] = value;
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
    return _data[index];
  }

  //---
  inline const T& operator[](u64 index) const
  {
    return _data[index];
  }

  //---
  inline T* emplace_back(u64 count = 1)
  {
    u64 original_count = _count;
    _count += count;
    return (_data + original_count);
  }

  //---
  inline void remove(u64 index)
  {
    --_count;
    memcpy_unsafe(_data + index, _data + _count, sizeof(T));
  }

  //---
  inline void remove(u64 index,
                     u64 count)
  {
    T* start = (_data + index);
    T* end = (_data + _count - count);
    _count -= count;
    memcpy_unsafe(start, end, sizeof(T) * count);
  }

  //---
  inline void clear()
  {
    _count = 0;
  }
};
}