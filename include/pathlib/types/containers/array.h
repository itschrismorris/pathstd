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
struct Array
{
private:
  //---
  alignas(32) T data[CAPACITY];
  u64 count;

public:
  //---
  Array()
  {
    clear();
  }
  ~Array() {}

  //---
  inline T& operator[](u64 index)
  {
    if (EXPECT(index < count)) {
      return data[index];
    } else {
      error.last_error.format(u8"Out of bounds access to Array.");
      error.to_log();
      error.fatality();
      return data[0];
    }
  }

  //---
  inline const T& operator[](u64 index) const
  {
    if (EXPECT(index < count)) {
      return data[index];
    } else {
      error.last_error.format(u8"Out of bounds access to Array.");
      error.to_log();
      error.fatality();
      return data[0];
    }
  }

  //---
  inline T* emplace_back(u64 _count = 1)
  {
    if (EXPECT(((count + _count) > count) &&
               (count + _count) <= CAPACITY)) {
      u64 original_count = count;
      count += _count;
      return (data + original_count);
    } else {
      error.last_error.format(u8"Failed to emplace_back() Array; it is already at capacity.");
      error.to_log();
      error.fatality();
      return nullptr;
    }
  }

  //---
  inline void remove(u64 index)
  {
    if (EXPECT((index < count) && (count > 0))) {
      --count;
      Memory::memcpy(index, data + count, sizeof(T));
    } else {
      error.last_error.format(u8"Failed to remove() from Array; invalid index.");
      error.to_log();
      error.fatality();
    }
  }

  //---
  inline void remove(u64 index,
                     u64 _count)
  {
    if (EXPECT(((index + _count) > index) &&
               ((index + _count) <= count))) {
      T* start = (data + index);
      T* end = (data + count - _count);
      count -= _count;
      Memory::memcpy(start, end, sizeof(T) * _count);
    } else {
      error.last_error.format(u8"Failed to remove() from Array; invalid index.");
      error.to_log();
      error.fatality();
    }
  }

  //---
  inline void clear()
  {
    count = 0;
  }
};
}