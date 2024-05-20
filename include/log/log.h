/*
  Documentation: https://www.path.blog/docs/log.html
*/

#pragma once
#include "types/types.h"
#include "win32/console.h"
#include "win32/safe_win32.h"
#include "types/string/long_string.h"

/**/
#define LOG(A) ::Pathlib::log.log(A)
#define LOGT(A) ::Pathlib::log.logt(A)

namespace Pathlib {

/**/
struct Log
{
  /**/
  void* file;
  String::LongString<> buffer;

  /**/
  Log() : file(nullptr) {}
  ~Log() {}
  bool initiate(const utf8* log_path);
  void shutdown();

  /**/
  template <typename... Args>
  inline bool log(Args&&... args)
  {
    if (file) {
      buffer.size = 0;
      (buffer._append(&buffer, args), ...);
      buffer.append(u8'\n');
      return (Console::write(buffer) && Win32::write_file(file, buffer.str, buffer.size));
    }
    return false;
  }

  /**/
  template <typename... Args>
  inline bool logt(Args&&... args)
  {
    if (file) {
      SystemTime time;
      Win32::get_local_time(&time);
      buffer.size = 0;
      buffer.append(time.wHour, u8":", time.wMinute, u8":", time.wSecond, u8":", time.wMilliseconds, u8": ");
      (buffer._append(&buffer, args), ...);
      buffer.append(u8'\n');
      return (Console::write(buffer) && Win32::write_file(file, buffer.str, buffer.size));
    }
    return false;
  }
};

/**/
extern Log log;
}