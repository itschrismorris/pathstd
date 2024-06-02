/*
  Documentation: https://www.path.blog/docs/pool.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"
#include "pathlib/memory/malloc_unsafe.h"

namespace Pathlib {

//---
template <typename T>
struct SafePtr
{
private:
  //---
  T* ptr;
  T* offset_ptr;
  u64 count;
  bool allocated_memory;

public:
  //---
  SafePtr() 
  {
    ptr = offset_ptr = nullptr;
    count = 0;
    allocated_memory = false;
  }

  //---
  SafePtr(T* _ptr,
          u64 _count)
  {
    
    ptr = offset_ptr = _ptr;
    if (_ptr == nullptr) {
      count = 0;
    } else {
      count = _count;
    }
    allocated_memory = false;
  }

  //---
  SafePtr(const T* _ptr)
  {
    ptr = offset_ptr = (T*)_ptr;
    if (_ptr == nullptr) {
      count = 0;
    }
    else {
      count = 1;
    }
    allocated_memory = false;
  }

  //---
  ~SafePtr() 
  {
    if (allocated_memory && is_valid()) {
      free_unsafe((void**)&ptr);
    }
  }

  //---
  template <typename U>
  operator U*() const 
  {
    return (U*)offset_ptr;
  }

  //---
  T* operator->()
  {
    if (EXPECT(this->ptr != nullptr)) {
      return this->ptr;
    } else {
      error.set_last_error(u8"Attempt to access an object through a null SafePtr.");
      error.to_log();
      error.kill_script();
      return &ptr[0];
    }
  }

  //---
  T& operator*() const 
  {
    if (EXPECT(offset_ptr != nullptr)) {
      return offset_ptr[0];
    } else {
      error.set_last_error(u8"Attempt to access a null SafePtr.");
      error.to_log();
      error.kill_script();
      return ptr[0];
    }
  }

  //---
  T& operator[](u64 index)
  {
    if (EXPECT(((offset_ptr - ptr + index) < count))) {
      return offset_ptr[index];
    } else {
      if (ptr == nullptr) {
        error.set_last_error(u8"Attempt to access a null SafePtr.");
      } else {
        error.set_last_error(u8"Out of bounds access to SafePtr.");
      }
      error.to_log();
      error.kill_script();
      return ptr[0];
    }
  }

  //---
  SafePtr& operator =(const SafePtr& _ptr)
  {
    ptr = _ptr.ptr;
    offset_ptr = _ptr.offset_ptr;
    count = _ptr.count;
    return *this;
  }

  //---
  SafePtr& operator =(T* _ptr)
  {
    ptr = offset_ptr = _ptr;
    if (_ptr == nullptr) {
      count = 0;
    } else {
      count = 1;
    }
    return *this;
  }

  //---
  SafePtr& operator =(const T* _ptr)
  {
    ptr = offset_ptr = _ptr;
    if (_ptr == nullptr) {
      count = 0;
    }
    else {
      count = 1;
    }
    return *this;
  }

  //---
  const SafePtr operator-(u64 offset)
  {
    if (EXPECT(((offset_ptr - offset) < offset_ptr) &&
               ((offset_ptr - offset) >= ptr))) {
      SafePtr new_ptr;
      new_ptr.ptr = ptr;
      new_ptr.offset_ptr = offset_ptr - offset;
      new_ptr.count = count;
      return new_ptr;
    } else {
      error.set_last_error(u8"Out of bounds pointer arithmetic; pointer must remain at, or after, original address.");
      error.to_log();
      error.kill_script();
      return SafePtr(nullptr, 0);
    }
  }

  //---
  const SafePtr operator+(u64 offset)
  {
    if (EXPECT(((offset_ptr + offset) > offset_ptr) &&
               ((offset_ptr + offset) < (ptr + count)))) {
      SafePtr new_ptr;
      new_ptr.ptr = ptr;
      new_ptr.offset_ptr = offset_ptr + offset;
      new_ptr.count = count;
      return new_ptr;
    } else {
      error.set_last_error(u8"Out of bounds pointer arithmetic.");
      error.to_log();
      error.kill_script();
      return SafePtr(nullptr, 0);
    }
  }

  //---
  void set_count(u64 _count)
  {
    count = _count;
  }

  //---
  void set_allocated_memory(bool _allocated_memory)
  {
    allocated_memory = _allocated_memory;
  }

  //---
  u64 get_count() const
  {
    return count;
  }

  //---
  const T* get_ptr() const
  {
    return ptr;
  }

  //---
  bool is_null() const
  {
    return ((ptr == nullptr) || (count == 0));
  }

  //---
  bool is_valid() const
  {
    return ((ptr != nullptr) && (count > 0));
  }
};
}