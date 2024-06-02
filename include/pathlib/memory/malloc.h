/*
  Documentation: https://www.path.blog/docs/malloc.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/containers/safe_ptr.h"
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/error/error.h"

namespace Pathlib {

//---
template <typename T>
SafePtr<T> malloc(u64 count,
                  const utf8* name = nullptr)
{
  if (Memory::_Internal::scripting_mode) {
    SafePtr<T> ptr = (T*)malloc_unsafe(count * sizeof(T), name);
    if (ptr.is_null()) {
      error.set_last_error(u8"Failed to malloc(); potentially out of memory.");
      error.to_log();
      error.kill_script();
      return nullptr;
    }
    ptr.set_count(count);
    ptr.set_allocated_memory(true);
    return ptr;
  } else {
    SafePtr<T> ptr = (T*)malloc_unsafe(count * sizeof(T), name);
    if (ptr.is_null()) {
      error.set_last_error(u8"Failed to malloc(); potentially out of memory.");
      error.to_log();
      error.kill_script();
      return nullptr;
    }
    ptr.set_count(count);
    ptr.set_allocated_memory(true);
    return ptr;
  }
}

//---
template <typename T>
SafePtr<T> realloc(SafePtr<T>& _ptr,
                   u64 count,
                   bool named = false)
{
  if (Memory::_Internal::scripting_mode) {
    if (_ptr.is_valid()) {
      SafePtr<T> ptr = (T*)realloc_unsafe(_ptr, count * sizeof(T), named);
      if (ptr.is_null()) {
        error.set_last_error(u8"Failed to realloc(); potentially out of memory.");
        error.to_log();
        error.kill_script();
        return nullptr;
      }
      ptr.set_count(count);
      ptr.set_allocated_memory(true);
      return ptr;
    } else {
      error.set_last_error(u8"Attempt to realloc() a null SafePtr.");
      error.to_log();
      error.kill_script();
    }
  } else {
    if (_ptr.is_valid()) {
      SafePtr<T> ptr = (T*)realloc_unsafe(_ptr, count * sizeof(T), named);
      if (ptr.is_null()) {
        error.set_last_error(u8"Failed to malloc(); potentially out of memory.");
        error.to_log();
        error.kill_script();
        return nullptr;
      }
      ptr.set_count(count);
      ptr.set_allocated_memory(true);
      return ptr;
    } else {
      error.set_last_error(u8"Attempt to realloc() a null SafePtr.");
      error.to_log();
      error.kill_script();
    }
  }
}

//---
template <typename T>
void free(SafePtr<T>& _ptr,
          bool named = false)
{
  if (Memory::_Internal::scripting_mode) {
    if (_ptr.is_valid()) {
      _ptr.set_allocated_memory(false);
      free_unsafe((void**)&_ptr, named);
    } else {
      error.set_last_error(u8"Attempt to free() a null SafePtr.");
      error.to_log();
      error.kill_script();
    }
  } else {
    if (_ptr.is_valid()) {
      _ptr.set_allocated_memory(false);
      free_unsafe((void**)&_ptr, named);
    } else {
      error.set_last_error(u8"Attempt to free() a null SafePtr.");
      error.to_log();
      error.kill_script();
    }
  }
}
}