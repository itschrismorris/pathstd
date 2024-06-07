/*
  Documentation: https://www.path.blog/docs/thread.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/concurrency/atomics.h"
#include "pathlib/concurrency/mpsc_queue.h"
#include "pathlib/concurrency/threadpool.h"
#include "pathlib/errors/errors.h"

namespace Pathlib {

//---
struct Spinlock
{
  //---
  static constexpr u32 NO_OWNER = Types::U32_MAX;

  //---
  MPSCQueue<volatile u32*, _Internal::Threadpool::MAX_THREAD_COUNT> _queue;
  Atomic<u32> _owner;

  //---
  Spinlock() 
  {
    _owner.store(NO_OWNER);
  }
  ~Spinlock() {}

  //
  [[nodiscard]] inline bool try_acquire()
  {
    return _owner.compare_and_swap(NO_OWNER, Win32::get_current_thread_id());
  }

  //---
  [[nodiscard]] inline bool acquire()
  {
    if (try_acquire()) {
      return true;
    } else {
      volatile u32 wait = 1;
      if (_queue.push(&wait)) {
        u32 spins = 0;
        while (wait) {
          if (++spins < 2000) {
            __builtin_ia32_pause();
          } else {
            Win32::yield_thread();
          }
        }
        _owner.store(Win32::get_current_thread_id());
        return true;
      } else {
        get_errors().to_log_with_stacktrace(u8"Spinlock _queue overflowed; could not acquire lock.");
        return false;
      }
    }
  }

  //---
  inline void release()
  {
    if (_owner.load() == Win32::get_current_thread_id()) {
      volatile u32* next;
      if (_queue.pop(next)) {
        *next = 0;
      } else {
        _owner.store(NO_OWNER);
      }
    }
  }
};
}