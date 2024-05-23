#include "pathlib/win32/safe_win32.h"
#include "pathlib/win32/console.h"
#include "pathlib/log/log.h"
#include "../src/win32/mindows.h"
#include "pathlib/error/error.h"

namespace Pathlib {

//---
Error error;

//---
bool Error::last_error_from_win32()
{
  if ((last_error.size = Win32::get_last_error_string(last_error.str, last_error.capacity)) == 0) {
    return false;
  }
  return true;
}

//---
bool Error::to_log(bool use_color)
{
  String::LongString callstack;
  if (!Win32::get_callstack(&callstack)) {
    return false;
  }
  String::LongString string(u8"\n************\n", last_error, u8'\n', callstack, u8"\n************\n");
  if (use_color) {
    if  (!console.set_text_attributes(CONSOLE_FOREGROUND_RED) ||
         !LOGT(string.str) ||
         !console.set_text_attributes(CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_BLUE)) {
      return false;
    }
    return true;
  } else {
    return LOGT(string.str);
  }
}

//---
bool Error::to_popup()
{
  return true;
}
} 