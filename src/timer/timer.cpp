#include "log/log.h"
#include "errors/errors.h"
#include "timer/timer.h"
#include "types.h"
#include "../src/lean_windows.h"

namespace Pathlib {

/**/
Error Timer::initiate(void)
{
  //LOG.log(L"Initiating timer.");
  LARGE_INTEGER ticks;
  QueryPerformanceFrequency(&ticks);
  ticks_per_second = ticks.QuadPart;
  LARGE_INTEGER now_ticks;
  QueryPerformanceCounter(&now_ticks);
  start_time = (now_ticks.QuadPart * 1000) / ticks_per_second;
  return Errors::NONE;
}

/**/
void Timer::shutdown(void)
{
  //LOG.log(L"Shutting down timer.");
}

/**/
u64 Timer::now_ms(void)
{
  LARGE_INTEGER now_ticks;
  QueryPerformanceCounter(&now_ticks);
  u64 now_ms = (now_ticks.QuadPart * 1000) / ticks_per_second;
  return (now_ms - start_time);
}

/**/
u64 Timer::now_us(void)
{
  LARGE_INTEGER now_ticks;
  QueryPerformanceCounter(&now_ticks);
  u64 now_us = (now_ticks.QuadPart * 1000000) / ticks_per_second;
  return (now_us - start_time);
}
}