/*
  Documentation: https://www.path.blog/docs/console.html
*/

#pragma once
#include "win32/safe_win32.h"
#include "types.h"
#include "string/local_string.h"

namespace Pathlib::Win32 {

/**/
template <u64 CAPACITY>
inline void print_arg(String::LocalString<CAPACITY>& arg)
{
  safe_write_console(arg.str, arg.size);
}

/**/
template <typename T>
inline void print_arg(const T arg)
{
  static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
  static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
  if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
    u64 length = String::strlen(arg);
    safe_write_console(arg, length);
  } else if constexpr (IS_INTEGRAL(T) || IS_FLOAT(T)) {
    utf8 formatted[32];
    u64 size = String::from_type(arg, formatted, 32);
    safe_write_console(formatted, size);
  } else if constexpr (IS_VEC2(T))  {
    utf8 formatted[64];
    u64 size = String::from_type(arg, formatted, 64);
    safe_write_console(formatted, size);
  } else if constexpr (IS_VEC3(T))  {
    utf8 formatted[96];
    u64 size = String::from_type(arg, formatted, 96);
    safe_write_console(formatted, size);
  } else if constexpr (IS_VEC4(T))  {
    utf8 formatted[192];
    u64 size = String::from_type(arg, formatted, 192);
    safe_write_console(formatted, size);
  } else {
    static_assert(false, "Unsupported type used for printing to console.");
  }
}

/**/
template <typename... Args>
static inline void console(Args&&... args)
{
  (print_arg(args), ...);
}
}