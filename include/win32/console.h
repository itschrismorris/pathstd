/*
  Documentation: https://www.path.blog/docs/console.html
*/

#pragma once
#include "win32/safe_win32.h"
#include "types/types.h"
#include "error/error.h"
#include "types/string/short_string.h"
#include "types/string/long_string.h"

namespace Pathlib::Console {

/**/
static inline bool set_text_attributes(u16 attributes)
{
  return Win32::set_console_text_attributes(attributes);
}

/**/
template <u64 CAPACITY>
static inline bool write(String::ShortString<CAPACITY>& string)
{
  return Win32::write_console(string.str, string.size);
}

/**/
template <u64 RESERVE_CAPACITY>
static inline bool write(String::LongString<RESERVE_CAPACITY>& string)
{
  return Win32::write_console(string.str, string.size);
}

/**/
template <u64 CAPACITY>
static inline bool write(const String::ShortString<CAPACITY>& string)
{
  return Win32::write_console(string.str, string.size);
}

/**/
template <u64 RESERVE_CAPACITY>
static inline bool write(const String::LongString<RESERVE_CAPACITY>& string)
{
  return Win32::write_console(string.str, string.size);
}

/**/
template <typename... Args>
static inline bool write(Args&&... args)
{
  String::LongString string;
  (string._append(&string, args), ...);
  string.append(u8'\n');
  return Win32::write_console(string.str, string.size);
}
}