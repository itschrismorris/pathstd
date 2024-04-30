/* 'std/log.h'

  + All logging includes timestamps.

    Path game engine: https://www.path.blog
*/

#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <time.h>
#include "types.h"

namespace Pathlib {

/**/
struct Log
{
  /**/
  FILE* file;

  /**/
  Log() : file(nullptr) {}
  ~Log() {}

  /**/
  Error initiate(const wchar_t* log_path);
  void shutdown();
  Error log(const wchar_t* message, ...);
  Error error(const wchar_t* message, ...);
};
}