/* 'mindows.h'

  + Minimal interface to Windows API functions we use.

    Path: https://www.path.blog
*/

#pragma once
#include "mindows.h"
#include "types.h"
#include "string/strlen.h"
#include "string/from_int.h"
#include "string/from_float.h"

namespace Pathlib::Win32 {

/**/
template <typename T>
void print_arg(void* out,
               T arg)
{
  static_assert(!SAME_TYPE(T, const char*), "Argument to Win32::console() must use utf8 type (example: u8\"Hello world\")");
  static_assert(!SAME_TYPE(T, char*), "Argument to Win32::console() must use utf8 type (example: u8\"Hello world\")");
  if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
    u64 length = String::strlen(arg);
    WriteConsoleA(out, arg, length, nullptr, nullptr);
  } else if constexpr (SAME_TYPE(T, i32) || SAME_TYPE(T, i64) || 
                       SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
    utf8 buffer[32];
    utf8* string = String::from_int(arg, buffer);
    u64 length = String::strlen(string);
    WriteConsoleA(out, string, length, nullptr, nullptr);
  } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
    utf8 buffer[32];
    utf8* string = String::from_float(arg, buffer);
    u64 length = String::strlen(string);
    WriteConsoleA(out, string, length, nullptr, nullptr);
  }
}

/**/
template <typename... Args>
static void console(Args... args)
{
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  (print_arg(out, args), ...);
}
}