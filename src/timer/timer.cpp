#include "pathlib/pathlib.h"
#include "../src/win32/mindows.h"

namespace Pathlib {

//---
_Internal::Timer& get_timer()
{
  static _Internal::Timer timer;
  return timer;
}
}

namespace Pathlib::_Internal {

//---
Timer::Timer()
{
  get_log().logt(u8"Initiating timer.");
  LARGE_INTEGER ticks;
  if (!QueryPerformanceFrequency(&ticks)) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().fatal(FixedStringUnsafe<256>(u8"Failed to initiate timer, QueryPerformanceFrequency() failed: ", win_err)._str);
  }
  _ticks_per_second = ticks.QuadPart;
  LARGE_INTEGER now_ticks;
  if (!QueryPerformanceCounter(&now_ticks)) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().fatal(FixedStringUnsafe<256>(u8"Failed to initiate timer, QueryPerformanceCounter() failed: ", win_err)._str);
  }
  _start_time = (now_ticks.QuadPart * 1000) / _ticks_per_second;
}

//---
Timer::~Timer()
{
}

//---
u64 Timer::now_ms(void)
{
  LARGE_INTEGER now_ticks;
  if (!QueryPerformanceCounter(&now_ticks)) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().fatal(FixedStringUnsafe<256>(u8"Failed to get now_ms(); QueryPerformanceCounter() failed: ", win_err)._str);
    return 0;
  }
  u64 now_ms = (now_ticks.QuadPart * 1000) / _ticks_per_second;
  return (now_ms - _start_time);
}

//---
u64 Timer::now_us(void)
{
  LARGE_INTEGER now_ticks;
  if (!QueryPerformanceCounter(&now_ticks)) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().fatal(FixedStringUnsafe<256>(u8"Failed to get now_us(); QueryPerformanceCounter() failed: ", win_err)._str);
    return 0;
  }
  u64 now_us = (now_ticks.QuadPart * 1000000) / _ticks_per_second;
  return (now_us - _start_time);
}
}