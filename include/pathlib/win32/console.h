/*
  Documentation: https://www.path.blog/docs/console.html
*/

#pragma once
#include "pathlib/win32/safe_win32.h"
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/types/string/long_string.h"

namespace Pathlib {
  
/**/
struct Console
{
  /**/
  String::LongString<> buffer;

  /**/
  inline bool set_text_attributes(u16 attributes)
  {
    return Win32::set_console_text_attributes(attributes);
  }

  /**/
  template <u64 CAPACITY>
  inline bool write(const String::ShortString<CAPACITY>& string)
  {
    return Win32::write_console(string.str, string.size) && Win32::write_console(u8"\n", 1);
  }

  /**/
  template <u64 RESERVE_CAPACITY>
  inline bool write(const String::LongString<RESERVE_CAPACITY>& string)
  {
    return Win32::write_console(string.str, string.size) && Win32::write_console(u8"\n", 1);
  }

  /**/
  inline bool write(const utf8* string)
  {
    return Win32::write_console(string, String::size_of(string)) && Win32::write_console(u8"\n", 1);
  }

  /**/
  template <typename... Args>
  inline bool write(Args&&... args)
  {
    buffer.size = 0;
    (buffer._append(&buffer, args), ...);
    buffer.append(u8'\n');
    return Win32::write_console(buffer.str, buffer.size);
  }
};

/**/
extern Console console;
}