/*
  Documentation: https://www.path.blog/docs/log.html
*/

#pragma once
#include "types/types.h"
#include "win32/console.h"
#include "win32/safe_win32.h"
#include "types/string/long_string.h"

/**/
#define LOG(A) if (::Pathlib::pathlib.log.file) { ::Pathlib::pathlib.log.log(A); }
#define LOGT(A) if (::Pathlib::pathlib.log.file) { ::Pathlib::pathlib.log.logt(A); }

namespace Pathlib {

/**/
struct Log
{
  /**/
  void* file;

  /**/
  Log() : file(nullptr) {}
  ~Log() {}
  bool initiate(const utf8* log_path);
  void shutdown();

  /**/
  template <typename... Args>
  inline bool log(Args&&... args)
  {
    String::LongString string;
    (string._append(&string, args), ...);
    string.append(u8'\n');
    return (Console::write(string) && Win32::write_file(file, string.str, string.size));
  }

  /**/
  template <typename... Args>
  inline bool logt(Args&&... args)
  {
    SystemTime time;
    Win32::get_local_time(&time);
    String::LongString string;
    string.append(time.wHour, u8":", time.wMinute, u8":", time.wSecond, u8":", time.wMilliseconds, u8": ");
    (string._append(&string, args), ...);
    string.append(u8'\n');
    return (Console::write(string) && Win32::write_file(file, string.str, string.size));
  }
};
}