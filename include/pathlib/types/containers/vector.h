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
struct Vector
{
private:
  //---
  T* data;
  u64 count;
  u64 capacity;

public:
  //---
  Vector()
  {
    capacity = RESERVE_CAPACITY;
    data = (T*)MALLOC(sizeof(T) * RESERVE_CAPACITY);
    clear();
  }

  //---
  ~Vector()
  {
    if (data) {
      for (u64 c = 0; c < count; ++c) {
        Memory::call_destructor<T>(&data[c]);
      }
      FREE(data);
    }
  }

  //---
  inline T& operator[](u64 index)
  {
    if (EXPECT(index < count)) {
      return data[index];
    } else {
      error.last_error = u8"Out of bounds access to Vector.";
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
      error.last_error = u8"Out of bounds access to Vector.";
      error.to_log();
      error.fatality();
      return data[0];
    }
  }

  //---
  inline SafePtr<T> emplace_back(u64 _count = 1)
  {
    if (EXPECT((count + _count) > count)) {
      u64 original_count = count;
      count += _count;
      if (count > capacity) {
        capacity = count * 1.5;
        data = (T*)REALLOC(data, sizeof(T) * capacity);
      }
      Memory::call_constructor<T>(data + original_count);
      return SafePtr<T>(data + original_count, 1);
    } else {
      error.last_error = u8"Failed to emplace_back() Vector; it is already at capacity.";
      error.to_log();
      error.fatality();
      return SafePtr<T>(nullptr, 0);
    }
  }

  //---
  inline void remove(u64 index)
  {
    if (EXPECT(index < count)) {
      Memory::call_destructor<T>(&data[index]);
      --count;
      Memory::memcpy(index, data + count, sizeof(T));
    } else {
      error.last_error = u8"Failed to remove() from Vector; index is out of bounds.";
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
      for (u64 c = index; c < _count; ++c) {
        Memory::call_destructor<T>(&data[c]);
      }
      T* start = (data + index);
      T* end = (data + count - _count);
      count -= _count;
      Memory::memcpy(start, end, sizeof(T) * _count);
    } else {
      error.last_error = u8"Failed to remove() from Vector; removal is out of bounds.";
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