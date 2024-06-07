/*
  Documentation: https://www.path.blog/docs/console.html
*/

#pragma once
#include "pathlib/win32/safe_win32.h"
#include "pathlib/types/types.h"
#include "pathlib/string/short_string.h"
#include "pathlib/string/long_string.h"
#include "pathlib/string/short_string_unsafe.h"
#include "pathlib/string/long_string_unsafe.h"

namespace Pathlib::_Internal {
  
//---
struct Console
{
  //---
  static constexpr u16 BLUE = 0x0001;
  static constexpr u16 GREEN = 0x0002;
  static constexpr u16 RED = 0x0004;
  static constexpr u16 WHITE = (RED | GREEN | BLUE);
  static constexpr u16 BG_BLUE = 0x0010;
  static constexpr u16 BG_GREEN = 0x0020;
  static constexpr u16 BG_RED = 0x0040;
  static constexpr u16 BG_WHITE = (BG_RED | BG_GREEN | BG_BLUE);

  //---
  Console();
  ~Console();
  bool set_text_color(u16 attributes);

  //---
  template <u64 CAPACITY>
  inline bool write(const ShortString<CAPACITY>& string)
  {
    return Win32::write_console(string._str, string._size) && 
           Win32::write_console(u8"\n", 1);
  }

  //---
  template <u64 CAPACITY>
  inline bool write(const ShortStringUnsafe<CAPACITY>& string)
  {
    return Win32::write_console(string._str, string._size) && Win32::write_console(u8"\n", 1);
  }

  //---
  template <u64 RESERVE_CAPACITY>
  inline bool write(const LongString<RESERVE_CAPACITY>& string)
  {
    return Win32::write_console(string._str, string._size) &&
           Win32::write_console(u8"\n", 1);
  }

  //---
  template <u64 RESERVE_CAPACITY>
  inline bool write(const LongStringUnsafe<RESERVE_CAPACITY>& string)
  {
    return Win32::write_console(string._str, string._size) && Win32::write_console(u8"\n", 1);
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
    LongStringUnsafe<512> buffer(nullptr);
    (buffer._append(buffer, args), ...);
    buffer.append(u8'\n');
    return Win32::write_console(buffer._str, buffer._size);
  }
};
}

//---
namespace Pathlib { _Internal::Console& get_console(); }