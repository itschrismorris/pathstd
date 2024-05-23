/*
  Documentation: https://www.path.blog/docs/short_vector.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memcpy.h"

namespace Pathlib::Containers {

// ---
template <typename T, size_t CAPACITY>
struct ShortVector
{
  // ---
  alignas(32) T data[CAPACITY];
  u64 count;

  // ---
  ShortVector()
  {
    clear();
  }
  ~ShortVector() {}

  // ---
  inline T& operator[](u64 index)
  {
    return data[index];
  }

  // ---
  inline const T& operator[](u64 index) const
  {
    return data[index];
  }

  // ---
  inline T* emplace_back(u64 _count)
  {
    u64 original_count = count;
    count += _count;
    return (data + original_count);
  }

  // ---
  inline T pop()
  {
    return *(data + (--count));
  }

  // ---
  inline void remove(u64 start_index,
                     u64 width)
  {
    T* start = (data + start_index);
    T* end = (data + count - width);
    count -= width;
    Memory::memcpy(start, end, width);
  }

  // ---
  inline void clear()
  {
    count = 0;
  }
};
}