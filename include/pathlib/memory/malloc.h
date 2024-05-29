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
    if (ptr.is_null()) {
      error.set_last_error(u8"Failed to malloc(); potentially out of memory.");
      error.to_log();
      error.fatality();
      return nullptr;
    }
    ptr.set_count(count);
    ptr.set_allocated_memory(true);
    return ptr;
  } else {
    Containers::SafePtr<T> ptr = (T*)MALLOC(count * sizeof(T));
    if (ptr.is_null()) {
      error.set_last_error(u8"Failed to malloc(); potentially out of memory.");
      error.to_log();
      error.fatality();
      return nullptr;
    }
    ptr.set_count(count);
    ptr.set_allocated_memory(true);
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
    if (_ptr.is_valid()) {
      Containers::SafePtr<T> ptr = (T*)REALLOC(_ptr, count * sizeof(T));
      if (ptr.is_null()) {
        error.set_last_error(u8"Failed to realloc(); potentially out of memory.");
        error.to_log();
        error.fatality();
        return nullptr;
      }
      ptr.set_count(count);
      ptr.set_allocated_memory(true);
      return ptr;
    } else {
      error.set_last_error(u8"Attempt to realloc() a null SafePtr.");
      error.to_log();
      error.fatality();
    }
  } else {
    if (_ptr.is_valid()) {
      Containers::SafePtr<T> ptr = (T*)REALLOC(_ptr, count * sizeof(T));
      if (ptr.is_null()) {
        error.set_last_error(u8"Failed to malloc(); potentially out of memory.");
        error.to_log();
        error.fatality();
        return nullptr;
      }
      ptr.set_count(count);
      ptr.set_allocated_memory(true);
      return ptr;
    } else {
      error.set_last_error(u8"Attempt to realloc() a null SafePtr.");
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
    if (_ptr.is_valid()) {
      _ptr.set_allocated_memory(false);
      FREE(_ptr);
    } else {
      error.set_last_error(u8"Attempt to free() a null SafePtr.");
      error.to_log();
      error.fatality();
    }
  } else {
    if (_ptr.is_valid()) {
      _ptr.set_allocated_memory(false);
      FREE(_ptr);
    } else {
      error.set_last_error(u8"Attempt to free() a null SafePtr.");
      error.to_log();
      error.fatality();
    }
  }
}
}