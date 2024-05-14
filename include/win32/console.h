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
  } else if constexpr (SAME_TYPE(T, i8) || SAME_TYPE(T, i16) || SAME_TYPE(T, i32) || SAME_TYPE(T, i64) || 
                       SAME_TYPE(T, u8) || SAME_TYPE(T, u16) || SAME_TYPE(T, u32) || SAME_TYPE(T, u64) ||
                       SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
    utf8 formatted[32];
    u64 size = String::format_type(formatted, 32, arg);
    safe_write_console(formatted, size);
  } else if constexpr (SAME_TYPE(T, i32_2) || SAME_TYPE(T, i32_3) || 
                       SAME_TYPE(T, u32_2) || SAME_TYPE(T, u32_3) ||
                       SAME_TYPE(T, f32_2) || SAME_TYPE(T, f32_3))  {
    utf8 formatted[64];
    u64 size = String::format_type(formatted, 64, arg);
    safe_write_console(formatted, size);
  } else if constexpr (SAME_TYPE(T, i32_4) || SAME_TYPE(T, u32_4) ||
                       SAME_TYPE(T, f32_4))  {
    utf8 formatted[96];
    u64 size = String::format_type(formatted, 96, arg);
    safe_write_console(formatted, size);
  } else if constexpr (SAME_TYPE(T, i32_8) || SAME_TYPE(T, u32_8) ||
                       SAME_TYPE(T, f32_8))  {
    utf8 formatted[192];
    u64 size = String::format_type(formatted, 192, arg);
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