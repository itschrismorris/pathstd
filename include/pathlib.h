/*
  Documentation: https://www.path.blog/docs/pathlib.html
*/

#pragma once
#include "log/log.h"
#include "errors/errors.h"
#include "win32/console.h"

namespace Pathlib {
  
/**/
struct Core
{
  /**/
  Pathlib::Log log;

  /**/
  Error initiate(const utf8* log_path = nullptr);
  void shutdown();
};
extern Core core;
}