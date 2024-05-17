/*
  Documentation: https://www.path.blog/docs/console.html
*/

#pragma once
#include "win32/safe_win32.h"
#include "types.h"
#include "string/short_string.h"
#include "string/long_string.h"

namespace Pathlib::Console {

/**/
template <u64 CAPACITY>
static inline void write(String::ShortString<CAPACITY>& string)
{
  Win32::write_console(string.str, string.size);
}

/**/
template <u64 RESERVE_CAPACITY>
static inline void write(String::LongString<RESERVE_CAPACITY>& string)
{
  Win32::write_console(string.str, string.size);
}

/**/
template <typename... Args>
static inline void write(Args&&... args)
{
  String::ShortString<256> string_out;
  (string_out._append(&string_out, args), ...);
  Win32::write_console(string_out.str, string_out.size);
}
}