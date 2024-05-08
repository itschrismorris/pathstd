/* 'mindows.h'

  + Minimal interface to Windows API functions we use.

    Path: https://www.path.blog
*/

#pragma once
#include "mindows.h"
#include "types.h"
#include "string/strlen.h"

namespace Pathlib::Win32 {

/**/
static void console(const char8_t* message)
{
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  u64 length = String::strlen(message);
  WriteConsoleA(out, message, length, nullptr, nullptr);
  CloseHandle(out);
}
}