/* 'std/timer/timer.h'


    Path: https://www.path.blog
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
  Error initiate();
  void shutdown();
  u64 now_ms();
  u64 now_us();
};
}