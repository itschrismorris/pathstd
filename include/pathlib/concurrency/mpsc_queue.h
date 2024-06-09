/*
  Documentation: https://www.path.blog/docs/thread.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/concurrency/atomic.h"

namespace Pathlib {

//---
template <typename T,
          u32 CAPACITY>
struct MPSCQueue
{
  static_assert(Math::is_pot(CAPACITY), "MPSCQueue CAPACITY must be a power of two.");
  static_assert(IS_POINTER(T), "MPSCQueue must have pointer type; it is designed to queue objects stored elsewhere.");

  //---
  alignas(CACHE_LINE_SIZE) Atomic<u32> _head;
  Atomic<T> _data[CAPACITY];
  alignas(CACHE_LINE_SIZE) u32 _tail;

  //---
  MPSCQueue() 
  {
    _head.store(0);
    _tail = 0;
    for (u32 o = 0; o < CAPACITY; ++o) {
      _data[o].store(nullptr);
    }
  }
  ~MPSCQueue() {}

  //---
  [[nodiscard]] bool push(const T object)
  {
    u32 index = _head.fetch_add(1, MemOrder::RELAXED) - 1;
    Atomic<T>* head = &_data[index & (CAPACITY - 1)];
    if (head->load(MemOrder::ACQUIRE)) {
      return false;
    }
    head->store(object, MemOrder::RELEASE);
    return true;
  }

  //---
  [[nodiscard]] bool pop(T& object)
  {
    u32 index = _tail;
    Atomic<T>* tail = &_data[index & (CAPACITY - 1)];
    if (!tail->load(MemOrder::ACQUIRE)) {
      return false;
    }
    object = tail->swap(nullptr, MemOrder::ACQ_REL);
    u32 head = _head.load(MemOrder::RELAXED);
    if (head >= (index + CAPACITY)) {
      _head.store(index + 1, MemOrder::RELEASE);
    }
    ++_tail;
    return true;
  }
};
}