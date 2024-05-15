/*
  Documentation: https://www.path.blog/docs/pathlib.html
*/

#pragma once
#include "log/log.h"
#include "errors/errors.h"
#include "win32/console.h"
#include "timer/timer.h"
#include "string/local_string.h"
#include "memory/malloc.h"

namespace Pathlib {
  
/**/
struct Core
{
  /**/
  Pathlib::Log log;
  Pathlib::Timer timer;

  /**/
  Error initiate(const utf8* log_path = nullptr);
  void shutdown();
};

/**/
inline Core core;
}