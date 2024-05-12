#include "../src/win32/mindows.h"
#include "win32/safe_win32.h"
#include "errors/errors.h"

namespace Pathlib::Win32 {

/**/
Error safe_write_console(utf8* string,
                        u32 length)
{
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  if (WriteConsoleA(out, string, length, nullptr, nullptr) == 0) {
    return Pathlib::Errors::ERROR_PRINT_TO_CONSOLE;
  }
  return Pathlib::Errors::ERROR_NONE;
}
}