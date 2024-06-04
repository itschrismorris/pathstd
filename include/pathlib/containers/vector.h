/*
  Documentation: https://www.path.blog/docs/vector.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memcpy.h"

namespace Pathlib {

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
  Vector(const utf8* name,
         u64 reserve_capacity = RESERVE_CAPACITY)
  {
    capacity = reserve_capacity;
    data = (T*)malloc_unsafe(sizeof(T) * reserve_capacity,
                             ShortStringUnsafe<96>(name, u8"::[T*]data").str);
    clear();
  }

  //---
  ~Vector()
  {
    if (data) {
      for (u64 c = 0; c < count; ++c) {
        Memory::call_destructor<T>(&data[c]);
      }
      free_unsafe((void**)&data);
    }
  }

  //---
  inline T& operator[](u64 index)
  {
    if (EXPECT(index < count)) {
      return data[index];
    } else {
      get_errors().set_last_error(u8"Out of bounds access to Vector.");
      get_errors().to_log();
      get_errors().kill_script();
      return data[0];
    }
  }

  //---
  inline const T& operator[](u64 index) const
  {
    if (EXPECT(index < count)) {
      return data[index];
    } else {
      get_errors().set_last_error(u8"Out of bounds access to Vector.");
      get_errors().to_log();
      get_errors().kill_script();
      return data[0];
    }
  }

  //---
  template <typename... Args>
  inline SafePtr<T> emplace_back(u64 _count = 1,
                                 Args&&... constructor_args)
  {
    if (EXPECT((count + _count) > count)) {
      u64 original_count = count;
      count += _count;
      if (count > capacity) {
        capacity = count * 1.5;
        data = (T*)realloc_unsafe(data, sizeof(T) * capacity);
      }
      Memory::call_constructor<T>(data + original_count, constructor_args...);
      return SafePtr<T>(data + original_count, 1);
    } else {
      get_errors().set_last_error(u8"Failed to emplace_back() Vector; it is already at capacity.");
      get_errors().to_log();
      get_errors().kill_script();
      return SafePtr<T>(nullptr, 0);
    }
  }

  //---
  inline void remove(u64 index)
  {
    if (EXPECT(index < count)) {
      Memory::call_destructor<T>(&data[index]);
      --count;
      memcpy_unsafe(index, data + count, sizeof(T));
    } else {
      get_errors().set_last_error(u8"Failed to remove() from Vector; index is out of bounds.");
      get_errors().to_log();
      get_errors().kill_script();
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
      memcpy_unsafe(start, end, sizeof(T) * _count);
    } else {
      get_errors().set_last_error(u8"Failed to remove() from Vector; removal is out of bounds.");
      get_errors().to_log();
      get_errors().kill_script();
    }
  }

  //---
  inline void clear()
  {
    count = 0;
  }

  //---
  inline u64 get_count()
  {
    return count;
  }
};
}