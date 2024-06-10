#pragma once
#include "pathlib/types/types.h"
#include "pathlib/concurrency/atomic.h"

namespace Pathlib::_Internal {

//---
struct Arena
{
  //---
  u8* _head;
  Atomic<u64> _tail;
  u64 _capacity;

  //---
  DISALLOW_COPY(Arena);
  Arena();
  ~Arena();

  //---
  bool allocate(u64 capacity);
  void free();
  void* push(u64 size);
  void pop(u64 size);
  void clear();
};
}