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
struct Array
{
private:
  //---
  alignas(32) T _data[CAPACITY];
  u64 _count;

public:

  //---
  Array()
  {
    clear();
  }

  //---
  DISALLOW_COPY(Array);
  
  ~Array() {}

  //---
  void initializer_list(u64 index) {}
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
  template <typename... Args>
  Array(Args... args)
  {
    static_assert(sizeof...(Args) <= CAPACITY, "Initializer list exceeds Array capacity.");
    initializer_list(0, args...);
  }

  //---
  inline T& operator[](u64 index)
  {
    if (EXPECT(index < CAPACITY)) {
      return _data[index];
    } else {
      get_errors().fatal(u8"Out of bounds access to Array.");
      return _data[0];
    }
  }

  //---
  inline const T& operator[](u64 index) const
  {
    if (EXPECT(index < CAPACITY)) {
      return _data[index];
    } else {
      get_errors().fatal(u8"Out of bounds access to Array.");
      return _data[0];
    }
  }

  //---
  inline T* emplace_back(u64 count = 1)
  {
    if (EXPECT(((_count + count) > _count) &&
               (_count + count) <= CAPACITY)) {
      u64 original_count = _count;
      _count += count;
      return (_data + original_count);
    } else {
      get_errors().fatal(u8"Failed to emplace_back() Array; it is already at capacity.");
      return nullptr;
    }
  }

  //---
  inline void remove(u64 index)
  {
    if (EXPECT(index < _count)) {
      --_count;
      memcpy_unsafe(_data + index, _data + _count, sizeof(T));
    } else {
      get_errors().fatal(u8"Failed to remove() from Array; invalid index.");
    }
  }

  //---
  inline void remove(u64 index,
                     u64 count)
  {
    if (EXPECT(((index + count) > index) &&
               ((index + count) <= _count))) {
      T* start = (_data + index);
      T* end = (_data + _count - count);
      _count -= count;
      memcpy_unsafe(start, end, sizeof(T) * count);
    } else {
      get_errors().fatal(u8"Failed to remove() from Array; invalid region.");
    }
  }

  //---
  inline void clear()
  {
    _count = 0;
  }

  //---
  inline u64 get_count()
  {
    return _count;
  }
};
}