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
  bool initiate();
  void shutdown();
  u64 now_ms();
  u64 now_us();
};
}

//---
namespace Pathlib { extern _Internal::Timer timer; }