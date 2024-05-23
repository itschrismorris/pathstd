/*
  Documentation: https://www.path.blog/docs/log.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/win32/console.h"
#include "pathlib/win32/safe_win32.h"
#include "pathlib/types/string/long_string.h"

//---
#define LOG(A) ::Pathlib::log.log(A)
#define LOGT(A) ::Pathlib::log.logt(A)

namespace Pathlib::_Internal {

//---
struct Log
{
  //---
  void* file;
  String::LongString<> buffer;

  //---
  Log()
  {
    file = nullptr;
  }
  ~Log() {}

  //---
  bool initiate(const utf8* log_path);
  void shutdown();

  //---
  template <u64 CAPACITY>
  inline bool log(const String::ShortString<CAPACITY>& string)
  {
    return (Win32::write_console(string.str, string.size) && Win32::write_console(u8"\n", 1)) &&
           (Win32::write_log(string.str, string.size) && Win32::write_log(u8"\n", 1));
  }

  //---
  template <u64 RESERVE_CAPACITY>
  inline bool log(const String::LongString<RESERVE_CAPACITY>& string)
  {
    return (Win32::write_console(string.str, string.size) && Win32::write_console(u8"\n", 1)) &&
           (Win32::write_log(string.str, string.size) && Win32::write_log(u8"\n", 1));
  }

  //---
  inline bool log(const utf8* string)
  {
    u64 size = String::size_of(string);
    return (Win32::write_console(string, size) && Win32::write_console(u8"\n", 1)) &&
           (Win32::write_log(string, size) && Win32::write_log(u8"\n", 1));
  }

  //---
  template <typename... Args>
  inline bool log(Args&&... args)
  {
    if (file) {
      buffer.size = 0;
      (buffer._append(&buffer, args), ...);
      buffer.append(u8'\n');
      return (Win32::write_console(buffer.str, buffer.size) && Win32::write_log(buffer.str, buffer.size));
    }
    return false;
  }

  //---
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
      return (Win32::write_console(buffer.str, buffer.size) && Win32::write_log(buffer.str, buffer.size));
    }
    return false;
  }
};
}

//---
namespace Pathlib { extern _Internal::Log log; }