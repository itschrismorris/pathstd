/*
  Documentation: https://www.path.blog/docs/vector.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memcpy.h"

namespace Pathlib::Containers {

//---
template <typename T, 
          u64 RESERVE_CAPACITY>
struct VectorUnsafe
{
  //---
  T* data;
  u64 count;
  u64 capacity;

  //---
  VectorUnsafe()
  {
    capacity = RESERVE_CAPACITY;
    data = (T*)MALLOC(sizeof(T) * RESERVE_CAPACITY);
    clear();
  }

  //---
  ~VectorUnsafe()
  {
    for (u64 c = 0; c < count; ++c) {
      Memory::call_destructor<T>(&data[c]);
    }
    FREE(data);
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
    if (count > capacity) {
      capacity = count * 1.5;
      data = (T*)REALLOC(data, sizeof(T) * capacity);
    }
    Memory::call_constructor<T>(data + original_count);
    return (data + original_count);
  }

  //---
  inline void remove(u64 index)
  {
    Memory::call_destructor<T>(&data[index]);
    --count;
    Memory::memcpy_unsafe(index, data + count, sizeof(T));
  }

  //---
  inline void remove(u64 index,
                     u64 _count)
  {
    for (u64 c = index; c < _count; ++c) {
      Memory::call_destructor<T>(&data[c]);
    }
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