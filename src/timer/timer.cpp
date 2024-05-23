#include "pathlib/pathlib.h"
#include "../src/win32/mindows.h"

//---
namespace Pathlib { _Internal::Timer timer; }

namespace Pathlib::_Internal {

//---
Timer timer;

//---
bool Timer::initiate(void)
{
  LOGT(u8"Initiating timer.");
  LARGE_INTEGER ticks;
  if (!QueryPerformanceFrequency(&ticks)) {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  ticks_per_second = ticks.QuadPart;
  LARGE_INTEGER now_ticks;
  if (!QueryPerformanceCounter(&now_ticks)) {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  start_time = (now_ticks.QuadPart * 1000) / ticks_per_second;
  return true;
}

//---
void Timer::shutdown(void)
{
  LOGT(u8"Shutting down timer.");
}

//---
u64 Timer::now_ms(void)
{
  LARGE_INTEGER now_ticks;
  if (!QueryPerformanceCounter(&now_ticks)) {
    error.last_error_from_win32();
    error.to_log();
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
    error.last_error_from_win32();
    error.to_log();
    return 0;
  }
  u64 now_us = (now_ticks.QuadPart * 1000000) / ticks_per_second;
  return (now_us - start_time);
}
}