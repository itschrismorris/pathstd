/* 'std/log.h'

  + All logging includes timestamps.

    Path: https://www.path.blog
*/

#pragma once
#include "types.h"

namespace Pathlib {

/**/
struct Log
{
  /**/
  //FILE* file;

  /**/
 // Log() : file(nullptr) {}
  ~Log() {}

  /**/
  Error initiate(const utf8* log_path);
  void shutdown();
  Error log(const utf8* message, ...);
  Error error(const utf8* message, ...);
};
}