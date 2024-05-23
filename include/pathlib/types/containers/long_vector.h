/*
  Documentation: https://www.path.blog/docs/long_vector.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memcpy.h"

namespace Pathlib::Containers {

//---
template <typename T, u64 RESERVE_CAPACITY = 128LLU>
struct LongVector
{
  //---
  T* data;
  u64 count;
  u64 capacity;

  //---
  LongVector()
  {
    capacity = RESERVE_CAPACITY;
    data = (T*)MALLOC(sizeof(T) * RESERVE_CAPACITY);
    clear();
  }

  //---
  ~LongVector()
  {
    if (data) {
      FREE(data);
    }
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
  inline T* emplace_back(u64 _count)
  {
    u64 original_count = count;
    count += _count;
    if (count > capacity) {
      capacity = count * 1.5;
      data = (T*)REALLOC(data, sizeof(T) * capacity);
    }
    return (data + original_count);
  }

  //---
  inline T pop()
  {
    return *(data + (--count));
  }

  //---
  inline void remove(u64 start_index,
                     u64 width)
  {
    T* start = (data + start_index);
    T* end = (data + count - width);
    count -= width;
    Memory::memcpy(start, end, width);
  }

  //---
  inline void clear()
  {
    count = 0;
  }
};
}