/*
  Documentation: https://www.path.blog/docs/console.html
*/

#pragma once
#include "pathlib/win32/safe_win32.h"
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/types/string/long_string.h"

namespace Pathlib::_Internal {
  
//---
struct Console
{
  //---
  static constexpr u16 FOREGROUND_BLUE = 0x0001;
  static constexpr u16 FOREGROUND_GREEN = 0x0002;
  static constexpr u16 FOREGROUND_RED = 0x0004;
  static constexpr u16 FOREGROUND_INTENSITY = 0x0008;
  static constexpr u16 FOREGROUND_WHITE = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
  static constexpr u16 BACKGROUND_BLUE = 0x0010;
  static constexpr u16 BACKGROUND_GREEN = 0x0020;
  static constexpr u16 BACKGROUND_RED = 0x0040;
  static constexpr u16 BACKGROUND_INTENSITY = 0x0080;
  static constexpr u16 BACKGROUND_WHITE = (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);

  //---
  String::LongString<256> _buffer;

  //---
  inline bool set_text_attributes(u16 attributes)
  {
    return Win32::set_console_text_attributes(attributes);
  }

  //---
  template <u64 CAPACITY>
  inline bool write(const String::ShortString<CAPACITY>& string)
  {
    return Win32::write_console(string.str, string.size) && Win32::write_console(u8"\n", 1);
  }

  //---
  template <u64 RESERVE_CAPACITY>
  inline bool write(const String::LongString<RESERVE_CAPACITY>& string)
  {
    return Win32::write_console(string.str, string.size) && Win32::write_console(u8"\n", 1);
  }

  //---
  inline bool write(const utf8* string)
  {
    return Win32::write_console(string, String::size_of(string)) && Win32::write_console(u8"\n", 1);
  }

  //---
  template <typename... Args>
  inline bool write(Args&&... args)
  {
    _buffer.size = 0;
    (_buffer._append(&_buffer, args), ...);
    _buffer.append(u8'\n');
    return Win32::write_console(_buffer.str, _buffer.size);
  }
};
}

//---
namespace Pathlib { extern _Internal::Console console; }