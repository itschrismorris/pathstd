/*
  Documentation: https://www.path.blog/docs/malloc.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/containers/safe_ptr.h"
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/errors/errors.h"

namespace Pathlib {

//---
template <typename T>
SafePtr<T> malloc(u64 count,
                  const utf8* name)
{
  if (Memory::_Internal::scripting_mode) {
    SafePtr<T> ptr = (T*)malloc_unsafe(count * sizeof(T), name);
    if (ptr.is_null()) {
      get_errors().to_log_with_stacktrace(u8"Failed to malloc(); potentially out of memory.");
      get_errors().kill_script();
      return nullptr;
    }
    ptr.set_count(count);
    ptr.set_allocated_memory(true);
    return ptr;
  } else {
    SafePtr<T> ptr = (T*)malloc_unsafe(count * sizeof(T), name);
    if (ptr.is_null()) {
      get_errors().to_log_with_stacktrace(u8"Failed to malloc(); potentially out of memory.");
      get_errors().kill_script();
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
                   u64 count)
{
  if (Memory::_Internal::scripting_mode) {
    if (_ptr.is_valid()) {
      SafePtr<T> ptr = (T*)realloc_unsafe(_ptr, count * sizeof(T));
      if (ptr.is_null()) {
        get_errors().to_log_with_stacktrace(u8"Failed to realloc(); potentially out of memory.");
        get_errors().kill_script();
        return nullptr;
      }
      ptr.set_count(count);
      ptr.set_allocated_memory(true);
      return ptr;
    } else {
      get_errors().to_log_with_stacktrace(u8"Attempt to realloc() a null SafePtr.");
      get_errors().kill_script();
    }
  } else {
    if (_ptr.is_valid()) {
      SafePtr<T> ptr = (T*)realloc_unsafe(_ptr, count * sizeof(T));
      if (ptr.is_null()) {
        get_errors().to_log_with_stacktrace(u8"Failed to malloc(); potentially out of memory.");
        get_errors().kill_script();
        return nullptr;
      }
      ptr.set_count(count);
      ptr.set_allocated_memory(true);
      return ptr;
    } else {
      get_errors().to_log_with_stacktrace(u8"Attempt to realloc() a null SafePtr.");
      get_errors().kill_script();
    }
  }
}

//---
template <typename T>
void free(SafePtr<T>& _ptr)
{
  if (Memory::_Internal::scripting_mode) {
    if (_ptr.is_valid()) {
      _ptr.set_allocated_memory(false);
      void* p = (void*)_ptr.get_ptr();
      free_unsafe((void**)&p);
    } else {
      get_errors().to_log_with_stacktrace(u8"Attempt to free() a null SafePtr.");
      get_errors().kill_script();
    }
  } else {
    if (_ptr.is_valid()) {
      _ptr.set_allocated_memory(false);
      void* p = (void*)_ptr.get_ptr();
      free_unsafe((void**)&p);
    } else {
      get_errors().to_log_with_stacktrace(u8"Attempt to free() a null SafePtr.");
      get_errors().kill_script();
    }
  }
}
}