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
  void* file;
  LongStringUnsafe<256> _buffer;

  //---
  Log(const utf8* log_path);
  ~Log();

  //---
  template <u64 CAPACITY>
  inline bool log(const ShortString<CAPACITY>& string)
  {
    return (Win32::write_console(string.str, string.size) && 
            Win32::write_console(u8"\n", 1) &&
            Win32::write_log(string.str, string.size) &&  
            Win32::write_log(u8"\n", 1));
  }

  //---
  template <u64 CAPACITY>
  inline bool log(const ShortStringUnsafe<CAPACITY>& string)
  {
    return (Win32::write_console(string.str, string.size) && 
            Win32::write_console(u8"\n", 1) &&
            Win32::write_log(string.str, string.size) && 
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
    return  (Win32::write_console(string.str, string.size) && 
             Win32::write_console(u8"\n", 1) &&
             Win32::write_log(string.str, string.size) && 
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
      get_errors().set_last_error(u8"Attempt to log() with a null pointer.");
      get_errors().to_log();
      return false;
    }
  }

  //---
  template <typename... Args>
  inline bool log(Args&&... args)
  {
    if (file) {
      _buffer.size = 0;
      (_buffer._append(_buffer, args), ...);
      _buffer.append(u8'\n');
      return (Win32::write_console(_buffer.str, _buffer.size) && 
              Win32::write_log(_buffer.str, _buffer.size));
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
      _buffer.size = 0;
      _buffer.append(time.wHour, u8":", time.wMinute, u8":", 
                     time.wSecond, u8":", time.wMilliseconds, u8": ");
      (_buffer._append(_buffer, args), ...);
      _buffer.append(u8'\n');
      return (Win32::write_console(_buffer.str, _buffer.size) && 
              Win32::write_log(_buffer.str, _buffer.size));
    }
    return false;
  }
};
}

//---
namespace Pathlib { _Internal::Log& get_log(); }