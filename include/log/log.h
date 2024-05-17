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
  static inline void log(Args&&... args)
  {
    String::ShortString<256> string_out;
    (string_out._append(&string_out, args), ...);
    WriteFile(log, string_out.str, string_out.size, nullptr, nullptr);
  }
};
}