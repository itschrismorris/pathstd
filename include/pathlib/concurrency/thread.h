/*
  Documentation: https://www.path.blog/docs/thread.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/win32/safe_win32.h"

namespace Pathlib {

//---
struct Thread
{
  //---
  static constexpr u32 INFINITE_TIMEOUT = 0xFFFFFFFF;

  //---
  HANDLE _thread;

  //---
  Thread() 
  {
    _thread = nullptr;
  }

  //---
  ~Thread() 
  {
    join(3000);
  }

  //---
  bool spawn(void* user_data,
             ulong(*thread_function)(void* user_data));
  bool join(u32 timeout = INFINITE_TIMEOUT,
            ulong* exit_code = nullptr);
  bool terminate();
};
}