/*
  Documentation: https://www.path.blog/docs/log.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/win32/console.h"
#include "pathlib/win32/safe_win32.h"
#include "pathlib/string/long_string_unsafe.h"

namespace Pathlib::_Internal {

//---
struct Log
{
  //---
  void* _file;

  //---
  Log(const utf8* log_path);
  ~Log();

  //---
  template <typename... Args>
  inline bool log(Args&&... args)
  {
    if (_file) {
      LongStringUnsafe<512> buffer(nullptr);
      (buffer._append(buffer, args), ...);
      buffer.append(u8'\n');
      return (Win32::write_console(buffer._str, buffer._size) &&
              Win32::write_log(buffer._str, buffer._size));
    }
    return false;
  }

  //---
  template <typename... Args>
  inline bool logt(Args&&... args)
  {
    if (_file) {
      LongStringUnsafe<512> buffer(nullptr);
      SystemTime time;
      Win32::get_local_time(&time);
      buffer.append(time.wHour, u8":", time.wMinute, u8":", 
                    time.wSecond, u8":", time.wMilliseconds, u8": ");
      (buffer._append(buffer, args), ...);
      buffer.append(u8'\n');
      return (Win32::write_console(buffer._str, buffer._size) && 
              Win32::write_log(buffer._str, buffer._size));
    }
    return false;
  }
};
}

//---
namespace Pathlib { _Internal::Log& get_log(); }