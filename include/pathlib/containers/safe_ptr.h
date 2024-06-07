/*
  Documentation: https://www.path.blog/docs/pool.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/errors/errors.h"
#include "pathlib/memory/malloc_unsafe.h"

namespace Pathlib {

//---
template <typename T>
struct SafePtr
{
private:
  //---
  T* _ptr;
  T* _offset_ptr;
  u64 _count;
  bool _allocated_memory;

public:
  //---
  SafePtr() 
  {
    _ptr = _offset_ptr = nullptr;
    _count = 0;
    _allocated_memory = false;
  }

  //---
  SafePtr(T* ptr,
          u64 count)
  {
    
    _ptr = _offset_ptr = ptr;
    if (ptr == nullptr) {
      _count = 0;
    } else {
      _count = count;
    }
    _allocated_memory = false;
  }

  //---
  SafePtr(const T* ptr)
  {
    _ptr = _offset_ptr = (T*)ptr;
    if (ptr == nullptr) {
      _count = 0;
    } else {
      _count = 1;
    }
    _allocated_memory = false;
  }

  //---
  ~SafePtr() 
  {
    if (_allocated_memory && is_valid()) {
      free_unsafe((void**)&_ptr);
    }
  }

  //---
  template <typename U>
  operator U*() const 
  {
    return (U*)_offset_ptr;
  }

  //---
  T* operator->()
  {
    if (EXPECT(this->_ptr != nullptr)) {
      return this->_ptr;
    } else {
      get_errors().to_log_with_stacktrace(u8"Attempt to access an object through a null SafePtr.");
      get_errors().kill_script();
      return &_ptr[0];
    }
  }

  //---
  T& operator*() const 
  {
    if (EXPECT(_offset_ptr != nullptr)) {
      return _offset_ptr[0];
    } else {
      get_errors().to_log_with_stacktrace(u8"Attempt to access a null SafePtr.");
      get_errors().kill_script();
      return _ptr[0];
    }
  }

  //---
  T& operator[](u64 index)
  {
    if (EXPECT(((_offset_ptr - _ptr + index) < _count))) {
      return _offset_ptr[index];
    } else {
      if (_ptr == nullptr) {
        get_errors().to_log_with_stacktrace(u8"Attempt to access a null SafePtr.");
      } else {
        get_errors().to_log_with_stacktrace(u8"Out of bounds access to SafePtr.");
      }
      get_errors().kill_script();
      return _ptr[0];
    }
  }

  //---
  SafePtr& operator =(const SafePtr& ptr)
  {
    _ptr = ptr._ptr;
    _offset_ptr = ptr._offset_ptr;
    _count = ptr._count;
    return *this;
  }

  //---
  SafePtr& operator =(T* ptr)
  {
    _ptr = _offset_ptr = ptr;
    if (ptr == nullptr) {
      _count = 0;
    } else {
      _count = 1;
    }
    return *this;
  }

  //---
  SafePtr& operator =(const T* ptr)
  {
    _ptr = _offset_ptr = ptr;
    if (ptr == nullptr) {
      _count = 0;
    } else {
      _count = 1;
    }
    return *this;
  }

  //---
  const SafePtr operator-(u64 offset)
  {
    if (EXPECT(((_offset_ptr - offset) < _offset_ptr) &&
               ((_offset_ptr - offset) >= _ptr))) {
      SafePtr new_ptr;
      new_ptr._ptr = _ptr;
      new_ptr._offset_ptr = _offset_ptr - offset;
      new_ptr._count = _count;
      return new_ptr;
    } else {
      get_errors().to_log_with_stacktrace(u8"Out of bounds pointer arithmetic; pointer must remain at, or after, original address.");
      get_errors().kill_script();
      return SafePtr(nullptr, 0);
    }
  }

  //---
  const SafePtr operator+(u64 offset)
  {
    if (EXPECT(((_offset_ptr + offset) > _offset_ptr) &&
               ((_offset_ptr + offset) < (_ptr + _count)))) {
      SafePtr new_ptr;
      new_ptr._ptr = _ptr;
      new_ptr._offset_ptr = _offset_ptr + offset;
      new_ptr._count = _count;
      return new_ptr;
    } else {
      get_errors().to_log_with_stacktrace(u8"Out of bounds pointer arithmetic.");
      get_errors().kill_script();
      return SafePtr(nullptr, 0);
    }
  }

  //---
  void set_count(u64 count)
  {
    _count = count;
  }

  //---
  void set_allocated_memory(bool allocated_memory)
  {
    _allocated_memory = allocated_memory;
  }

  //---
  u64 get_count() const
  {
    return _count;
  }

  //---
  const T* get_ptr() const
  {
    return _ptr;
  }

  //---
  bool is_null() const
  {
    return ((_ptr == nullptr) || (_count == 0));
  }

  //---
  bool is_valid() const
  {
    return ((_ptr != nullptr) && (_count > 0));
  }
};
}