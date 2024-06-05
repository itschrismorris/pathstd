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
struct SPSCQueue
{
  static_assert(Math::is_pot(CAPACITY), "SPSCQueue CAPACITY must be a power of two.");
  static_assert(IS_POINTER(T), "SPSCQueue must have pointer type; it is designed to queue objects stored elsewhere.");

  //---
  alignas(CACHE_LINE_SIZE) u32 _head;
  Atomic<T> _data[CAPACITY];
  alignas(CACHE_LINE_SIZE) u32 _tail;

  //---
  SPSCQueue()
  {
    _head = 0;
    _tail = 0;
    for (u32 o = 0; o < CAPACITY; ++o) {
      _data[o].store(nullptr);
    }
  }
  ~SPSCQueue() {}

  //---
  [[nodiscard]] bool push(const T object)
  {
    Atomic<T>* head = &_data[_head & (CAPACITY - 1)];
    if (head->load(MemOrder::ACQUIRE)) {
      return false;
    }
    head->store(object, MemOrder::RELEASE);
    ++_head;
    return true;
  }
    
  //---
  [[nodiscard]] bool pop(T& object)
  {
    Atomic<T>* tail = &_data[_tail & (CAPACITY - 1)];
    if (!tail->load(MemOrder::ACQUIRE)) {
      return false;
    }
    object = tail->swap(nullptr, MemOrder::ACQ_REL);
    ++_tail;
    return true;
  }
};
}