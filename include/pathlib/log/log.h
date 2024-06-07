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
  template <u64 CAPACITY>
  inline bool log(const ShortString<CAPACITY>& string)
  {
    return (Win32::write_console(string._str, string._size) && 
            Win32::write_console(u8"\n", 1) &&
            Win32::write_log(string._str, string._size) &&  
            Win32::write_log(u8"\n", 1));
  }

  //---
  template <u64 CAPACITY>
  inline bool log(const ShortStringUnsafe<CAPACITY>& string)
  {
    return (Win32::write_console(string._str, string._size) && 
            Win32::write_console(u8"\n", 1) &&
            Win32::write_log(string._str, string._size) && 
            Win32::write_log(u8"\n", 1));
  }

  //---
  template <u64 RESERVE_CAPACITY>
  inline bool log(const LongString<RESERVE_CAPACITY>& string)
  {
    return (Win32::write_console(string._str, string._size) && 
            Win32::write_console(u8"\n", 1) &&
            Win32::write_log(string._str, string._size) &&
            Win32::write_log(u8"\n", 1));
  }

  //---
  template <u64 RESERVE_CAPACITY>
  inline bool log(const LongStringUnsafe<RESERVE_CAPACITY>& string)
  {
    return  (Win32::write_console(string._str, string._size) && 
             Win32::write_console(u8"\n", 1) &&
             Win32::write_log(string._str, string._size) && 
             Win32::write_log(u8"\n", 1));
  }

  //---
  inline bool log(const utf8* string)
  {
    if (EXPECT(string != nullptr)) {
      u64 size = String::size_of(string);
      return  (Win32::write_console(string, size) && 
               Win32::write_console(u8"\n", 1) &&
               Win32::write_log(string, size) && 
               Win32::write_log(u8"\n", 1));
    } else {
      get_errors().to_log(u8"Attempt to log() with a null pointer.");
      return false;
    }
  }

  //---
  template <typename... Args>
  inline bool log(Args&&... args)
  {
    if (_file) {
      ShortStringUnsafe<512> buffer;
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
      ShortStringUnsafe<512> buffer;
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