#include "pathlib/win32/safe_win32.h"
#include "pathlib/win32/console.h"
#include "pathlib/log/log.h"
#include "../src/win32/mindows.h"
#include "pathlib/memory/memory.h"
#include "pathlib/error/error.h"

namespace Pathlib { _Internal::Error error; }

namespace Pathlib::_Internal {

//---
void Error::set_last_error(const utf8* string)
{
  u64 size = Math::min(MAX_ERROR_LENGTH - 1, String::size_of(string));
  memcpy_unsafe(last_error, string, size);
  last_error[size] = u8'\0';
}

//---
bool Error::last_error_from_win32()
{
  if (Win32::get_last_error_string(last_error, 512) == 0) {
    return false;
  }
  return true;
}

//---
bool Error::to_log(bool use_color)
{
  if (!Win32::get_callstack(_buffer, MAX_ERROR_LENGTH)) {
    return false;
  }
  LongStringUnsafe<512> string(u8"\n************\n", last_error, u8"\n\n", _buffer, u8"************");
  if (use_color) {
    if  (!console.set_text_attributes(Win32::ConsoleColors::RED) ||
         !log.logt(string.str) ||
         !console.set_text_attributes(Win32::ConsoleColors::WHITE)) {
      return false;
    }
    return true;
  } else {
    return log.logt(string.str);
  }
}

//---
bool Error::to_popup()
{
  return true;
}

//
void Error::kill_script()
{
  if (Memory::_Internal::scripting_mode) {
    ExitProcess(0);
  } else {
    ExitProcess(0);
  }
}
} 