/*
  Documentation: https://www.path.blog/docs/malloc.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/containers/safe_ptr.h"
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/error/error.h"

namespace Pathlib::Memory {

//---
template <typename T>
Containers::SafePtr<T> malloc(u64 count,
                              const utf8* name = nullptr)
{
  if (Memory::_Internal::scripting_mode) {
    Containers::SafePtr<T> ptr = (T*)MALLOC(count * sizeof(T));
    ptr.set_count(count);
    return ptr;
  } else {
    Containers::SafePtr<T> ptr = (T*)MALLOC(count * sizeof(T));
    ptr.set_count(count);
    return ptr;
  }
}

//---
template <typename T>
Containers::SafePtr<T> realloc(Containers::SafePtr<T>& _ptr,
                               u64 count,
                               const utf8* name = nullptr)
{
  if (Memory::_Internal::scripting_mode) {
    if (!_ptr.is_null()) {
      Containers::SafePtr<T> ptr = (T*)REALLOC(_ptr, count * sizeof(T));
      ptr.set_count(count);
      return ptr;
    } else {
      error.last_error.format(u8"Attempt to realloc() a null SafePtr.");
      error.to_log();
      error.fatality();
    }
  } else {
    if (!_ptr.is_null()) {
      Containers::SafePtr<T> ptr = (T*)REALLOC(_ptr, count * sizeof(T));
      ptr.set_count(count);
      return ptr;
    } else {
      error.last_error.format(u8"Attempt to realloc() a null SafePtr.");
      error.to_log();
      error.fatality();
    }
  }
}

//---
template <typename T>
void free(Containers::SafePtr<T>& _ptr)
{
  if (Memory::_Internal::scripting_mode) {
    if (!_ptr.is_null()) {
      FREE(_ptr);
    } else {
      error.last_error.format(u8"Attempt to free() a null SafePtr.");
      error.to_log();
      error.fatality();
    }
  } else {
    if (!_ptr.is_null()) {
      FREE(_ptr);
    } else {
      error.last_error.format(u8"Attempt to free() a null SafePtr.");
      error.to_log();
      error.fatality();
    }
  }
}
}