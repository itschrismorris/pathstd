/*
  Documentation: https://www.path.blog/docs/timer.html
*/

#pragma once
#include "types.h"

namespace Pathlib {

/**/
struct Timer
{
  /**/
  u64 start_time;
  u64 ticks_per_second;

  /**/
  bool initiate();
  void shutdown();
  u64 now_ms();
  u64 now_us();
};
}