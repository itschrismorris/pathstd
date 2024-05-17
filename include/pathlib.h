/*
  Documentation: https://www.path.blog/docs/pathlib.html
*/

#pragma once
#include "log/log.h"
#include "errors/errors.h"
#include "win32/console.h"
#include "timer/timer.h"
#include "string/short_string.h"
#include "string/long_string.h"
#include "memory/malloc.h"
#include "win32/safe_win32.h"

namespace Pathlib {
  
/**/
struct Core
{
  /**/
  Pathlib::Log log;
  Pathlib::Timer timer;

  /**/
  bool initiate(const utf8* log_path = nullptr);
  void shutdown();
};

/**/
inline Core core;
}