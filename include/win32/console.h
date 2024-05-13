/*
  Documentation: https://www.path.blog/docs/console.html
*/

#pragma once
#include "win32/safe_win32.h"
#include "types.h"
#include "string/strlen.h"
#include "string/from_int.h"
#include "string/from_float.h"

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
                       SAME_TYPE(T, u8) || SAME_TYPE(T, u16) || SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
    utf8 buffer[32];
    u32 length;
    utf8* string = String::from_int(arg, buffer, &length);
    safe_write_console(string, length);
  } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
    utf8 buffer[32];
    u32 length;
    utf8* string = String::from_float(arg, buffer, &length);
    safe_write_console(string, length);
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