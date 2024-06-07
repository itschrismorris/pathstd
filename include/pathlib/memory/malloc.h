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
  T* ptr = (T*)malloc_unsafe(count * sizeof(T), name);
  if (!ptr) {
    get_errors().to_log_with_stacktrace(u8"Failed to malloc(); potentially out of memory.");
    get_errors().kill_script();
    return SafePtr<T>(nullptr, 0, false);
  }
  return SafePtr<T>(ptr, count, true);
}

//---
template <typename T>
SafePtr<T> realloc(SafePtr<T>& _ptr,
                   u64 count)
{
  if (_ptr.is_valid()) {
    T* ptr = (T*)realloc_unsafe(_ptr, count * sizeof(T));
    if (!ptr) {
      get_errors().to_log_with_stacktrace(u8"Failed to malloc(); potentially out of memory.");
      get_errors().kill_script();
      return SafePtr<T>(nullptr, 0, false);
    }
    return SafePtr<T>(ptr, count, true);
  } else {
    get_errors().to_log_with_stacktrace(u8"Attempt to realloc() a null SafePtr.");
    get_errors().kill_script();
    return SafePtr<T>(nullptr, 0, false);
  }
}

//---
template <typename T>
void free(SafePtr<T>& _ptr)
{
  if (_ptr.is_valid() && _ptr.get_holds_allocation()) {
    void* p = (void*)_ptr.get_ptr();
    free_unsafe((void**)&p);
    _ptr = SafePtr<T>(nullptr, 0, false);
  } else {
    get_errors().to_log_with_stacktrace(u8"Attempt to free() a null SafePtr.");
    get_errors().kill_script();
  }
}
}