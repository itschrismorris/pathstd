/*
  Documentation: https://www.path.blog/docs/vector.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memcpy.h"
#include "pathlib/string/short_string_unsafe.h"

namespace Pathlib {

//---
template <typename T, 
          u64 RESERVE_CAPACITY>
struct VectorUnsafe
{
  //---
  T* _data;
  u64 _count;
  u64 _capacity;

  //---
  VectorUnsafe(const utf8* name,
               u64 reserve_capacity = RESERVE_CAPACITY)
  {
    _capacity = reserve_capacity;
    _data = (T*)malloc_unsafe(sizeof(T) * reserve_capacity,
                              name ? ShortStringUnsafe<96>(name, u8"::[T*]data")._str : nullptr);
    clear();
  }

  //---
  ~VectorUnsafe()
  {
    for (u64 c = 0; c < _count; ++c) {
      Memory::call_destructor<T>(&_data[c]);
    }
    free_unsafe((void**)&_data);
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
  template <typename... Args>
  inline T* emplace_back(u64 count = 1,
                         Args&&... constructor_args)
  {
    u64 original_count = _count;
    _count += count;
    if (_count > _capacity) {
      _capacity = _count * 1.5;
      _data = (T*)realloc_unsafe(_data, sizeof(T) * _capacity);
    }
    Memory::call_constructor<T>(_data + original_count, constructor_args...);
    return (_data + original_count);
  }

  //---
  inline void remove(u64 index)
  {
    Memory::call_destructor<T>(&_data[index]);
    --_count;
    memcpy_unsafe(index, _data + _count, sizeof(T));
  }

  //---
  inline void remove(u64 index,
                     u64 count)
  {
    for (u64 c = index; c < count; ++c) {
      Memory::call_destructor<T>(&_data[c]);
    }
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