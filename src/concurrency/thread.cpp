#include "pathlib/win32/console.h"
#include "../src/win32/mindows.h"
#include "pathlib/concurrency/thread.h"
#include "pathlib/errors/errors.h"

namespace Pathlib {

//---
bool Thread::spawn(void* user_data,
                   ulong(*thread_function)(void* user_data))
{
  _thread = CreateThread(nullptr, 0, (DWORD(*)(void*))thread_function, user_data, 0, nullptr);
  if (!_thread) {
    get_errors().to_log_with_stacktrace(u8"Failed to spawn thread.");
    return false;
  }
  return true;
}

//---
bool Thread::join(u32 timeout,
                  ulong* exit_code)
{
  DWORD status = WaitForSingleObject(_thread, timeout);
  if (status == WAIT_TIMEOUT) {
    TerminateThread(_thread, 0);
  } else if (status == WAIT_OBJECT_0) {
    if (exit_code) {
      GetExitCodeThread(_thread, exit_code);
    }
  }
  CloseHandle(_thread);
  _thread = nullptr;
  return true;
}

//---
bool Thread::terminate()
{
  if (_thread) {
    TerminateThread(_thread, 0);
    CloseHandle(_thread);
    _thread = nullptr;
  }
  return true;
}
}