/*
  Documentation: https://www.path.blog/docs/thread.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/concurrency/atomics.h"

namespace Pathlib {

//---
template <typename T,
          u32 CAPACITY>
struct MPSCQueue
{
  static_assert(Math::is_pot(CAPACITY), "MPSCQueue CAPACITY must be a power of two.");

  //
  struct LockedObject
  {
    AtomicFlag locked;
    T object;
  };

  //---
  alignas(CACHE_LINE_SIZE) Atomic<u32> _head;
  LockedObject _data[CAPACITY];
  alignas(CACHE_LINE_SIZE) Atomic<u32> _tail;

  //---
  MPSCQueue() 
  {
    _head = 0;
    _tail = 0;
    for (u32 o = 0; o < CAPACITY; ++o) {
      _data[o].locked.clear();
    }
  }
  ~MPSCQueue() {}

  //---
  [[nodiscard]] bool push(const T& object)
  {
    u32 index = _head.load();
    LockedObject* head = &_data[index & (CAPACITY - 1)];
    if (head->locked.test_and_set()) {
      return false;
    }
    head->object = object;
    _head.fetch_add(1);
    return true;
  }

  //---
  [[nodiscard]] bool pop(T& object)
  {
    u32 index = _tail.load();
    LockedObject* tail = &_data[index & (CAPACITY - 1)];
    if (!tail->locked.test()) {
      return false;
    }
    object = tail->object;
    tail->locked.clear();
    _tail.fetch_add(1);
    return true;
  }
};
}