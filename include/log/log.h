/*
  Documentation: https://www.path.blog/docs/log.html
*/

#pragma once
#include "types.h"
#include "win32/safe_win32.h"

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

  /**/
  template <typename... Args>
  inline void log(Args&&... args)
  {
    String::LongString string;
    (string._append(&string, args), ...);
    Win32::write_file(file, string.str, string.size);
  }
};
}