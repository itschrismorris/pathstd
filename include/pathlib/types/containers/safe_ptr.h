/*
  Documentation: https://www.path.blog/docs/pool.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"

namespace Pathlib::Containers {

//---
template <typename T>
struct SafePtr
{
private:
  //---
  T* ptr;
  T* offset_ptr;
  u64 count;

public:
  //---
  SafePtr() 
  {
    ptr = offset_ptr = nullptr;
    count = 0;
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
  }

  //---
  SafePtr(T* _ptr)
  {
    ptr = offset_ptr = _ptr;
    if (_ptr == nullptr) {
      count = 0;
    } else {
      count = 1;
    }
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
  }

  //---
  ~SafePtr() {}

  //---
  operator void*() const {
    return (void*)offset_ptr;
  }

  //---
  operator utf8*() const {
    return (utf8*)offset_ptr;
  }

  //---
  T* operator->()
  {
    if (EXPECT(this->ptr != nullptr)) {
      return this->ptr;
    } else {
      error.last_error = u8"Attempt to access an object through a null SafePtr.";
      error.to_log();
      error.fatality();
      return &ptr[0];
    }
  }

  //---
  T& operator*() const 
  {
    if (EXPECT(offset_ptr != nullptr)) {
      return offset_ptr[0];
    } else {
      error.last_error = u8"Attempt to access a null SafePtr.";
      error.to_log();
      error.fatality();
      return ptr[0];
    }
  }

  //---
  inline T& operator[](u64 index)
  {
    if (EXPECT(((offset_ptr - ptr + index) < count))) {
      return offset_ptr[index];
    } else {
      if (ptr == nullptr) {
        error.last_error = u8"Attempt to access a null SafePtr.";
      } else {
        error.last_error = u8"Out of bounds access to SafePtr.";
      }
      error.to_log();
      error.fatality();
      return ptr[0];
    }
  }

  //---
  inline SafePtr& operator =(const SafePtr& _ptr)
  {
    ptr = _ptr.ptr;
    offset_ptr = _ptr.offset_ptr;
    count = _ptr.count;
    return *this;
  }

  //---
  inline SafePtr& operator =(T* _ptr)
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
  inline SafePtr& operator =(const T* _ptr)
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
  inline const SafePtr operator-(u64 offset)
  {
    if (EXPECT(((offset_ptr - offset) < offset_ptr) &&
               ((offset_ptr - offset) >= ptr))) {
      SafePtr new_ptr;
      new_ptr.ptr = ptr;
      new_ptr.offset_ptr = offset_ptr - offset;
      new_ptr.count = count;
      return new_ptr;
    } else {
      error.last_error = u8"Out of bounds pointer arithmetic; pointer must remain at, or after, original address.";
      error.to_log();
      error.fatality();
      return SafePtr(nullptr, 0);
    }
  }

  //---
  inline const SafePtr operator+(u64 offset)
  {
    if (EXPECT(((offset_ptr + offset) > offset_ptr) &&
               ((offset_ptr + offset) < (ptr + count)))) {
      SafePtr new_ptr;
      new_ptr.ptr = ptr;
      new_ptr.offset_ptr = offset_ptr + offset;
      new_ptr.count = count;
      return new_ptr;
    } else {
      error.last_error = u8"Out of bounds pointer arithmetic.";
      error.to_log();
      error.fatality();
      return SafePtr(nullptr, 0);
    }
  }

  //---
  inline void set_count(u64 _count)
  {
    count = _count;
  }

  //---
  inline bool is_null()
  {
    return ((ptr == nullptr) || (count == 0));
  }
};
}