#include "pathlib/win32/safe_win32.h"
#include "pathlib/win32/console.h"
#include "pathlib/log/log.h"
#include "../src/win32/mindows.h"
#include "pathlib/error/error.h"

namespace Pathlib { _Internal::Error error; }

namespace Pathlib::_Internal {

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
  if (!Win32::get_callstack(&_buffer)) {
    return false;
  }
  String::LongString<512> string(u8"\n************\n", last_error, u8"\n\n", _buffer, u8"************");
  if (use_color) {
    if  (!console.set_text_attributes(Console::FOREGROUND_RED) ||
         !LOGT(string.str) ||
         !console.set_text_attributes(Console::FOREGROUND_RED | Console::FOREGROUND_GREEN | Console::FOREGROUND_BLUE)) {
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