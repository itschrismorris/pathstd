/*
  Documentation: https://www.path.blog/docs/thread.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/concurrency/atomics.h"
#include "pathlib/concurrency/mpsc_queue.h"
#include "pathlib/concurrency/threadpool.h"

namespace Pathlib {

//---
struct Spinlock
{
  //---
  MPSCQueue<u8*, _Internal::Threadpool::MAX_THREAD_COUNT> queue;
  AtomicFlag _locked;

  //---
  Spinlock() 
  {
    _locked.clear();
  }
  ~Spinlock() {}

  //
  [[nodiscard]] inline bool try_acquire()
  {
    return !_locked.test_and_set();
  }

  //---
  inline bool acquire()
  {
    if (try_acquire()) {
      return true;
    } else {
      AtomicFlag wait;
      wait.test_and_set();
      // Place 'wait' pointer onto the queue so whoever currently
      // has the lock can unlock it for us.
      while (wait.test_and_set()) {

      }
    }
  }

  //---
  inline void release()
  {

  }
};
}