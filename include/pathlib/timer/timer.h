/*
  Documentation: https://www.path.blog/docs/timer.html
*/

#pragma once
#include "pathlib/types/types.h"

namespace Pathlib::_Internal {

//---
struct Timer
{
  //---
  u64 start_time;
  u64 ticks_per_second;

  //---
  Timer();
  ~Timer();
  u64 now_ms();
  u64 now_us();
};
}

//---
namespace Pathlib { _Internal::Timer& get_timer(); }