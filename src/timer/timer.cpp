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
    get_errors().last_error_from_win32();
    get_errors().to_log();
  }
  ticks_per_second = ticks.QuadPart;
  LARGE_INTEGER now_ticks;
  if (!QueryPerformanceCounter(&now_ticks)) {
    get_errors().last_error_from_win32();
    get_errors().to_log();
  }
  start_time = (now_ticks.QuadPart * 1000) / ticks_per_second;
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
    get_errors().last_error_from_win32();
    get_errors().to_log();
    return 0;
  }
  u64 now_ms = (now_ticks.QuadPart * 1000) / ticks_per_second;
  return (now_ms - start_time);
}

//---
u64 Timer::now_us(void)
{
  LARGE_INTEGER now_ticks;
  if (!QueryPerformanceCounter(&now_ticks)) {
    get_errors().last_error_from_win32();
    get_errors().to_log();
    return 0;
  }
  u64 now_us = (now_ticks.QuadPart * 1000000) / ticks_per_second;
  return (now_us - start_time);
}
}