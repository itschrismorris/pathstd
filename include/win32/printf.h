/* 'mindows.h'

  + Minimal interface to Windows API functions we use.

    Path: https://www.path.blog
*/

#pragma once
#include "mindows.h"

namespace Pathlib::Win32 {

static void printf(const char* message)
{
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  unsigned long written = 0;
  WriteFile(out, L"Hello World!!", 26, &written, nullptr);
  CloseHandle(out);
}
}