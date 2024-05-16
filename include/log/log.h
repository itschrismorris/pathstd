/*
  Documentation: https://www.path.blog/docs/log.html
*/

#pragma once
#include "../src/win32/mindows.h"
#include "types.h"
#include "errors/errors.h"

namespace Pathlib {

/**/
struct Log
{
  /**/
  HANDLE file;

  /**/
  Log() : file(nullptr) {}
  ~Log() {}
  bool initiate(const utf8* log_path);
  void shutdown();
};
}