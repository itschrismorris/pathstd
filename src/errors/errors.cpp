#include "pathlib/win32/safe_win32.h"
#include "pathlib/win32/console.h"
#include "pathlib/log/log.h"
#include "../src/win32/mindows.h"
#include "pathlib/memory/memory.h"
#include "pathlib/errors/errors.h"

namespace Pathlib {

//---
_Internal::Errors& get_errors()
{
  static _Internal::Errors errors;
  return errors;
}
}


namespace Pathlib::_Internal {

//---
void Errors::set_last_error(const utf8* string)
{
  u64 size = Math::min(MAX_ERROR_LENGTH - 1, String::size_of(string));
  memcpy_unsafe(_last_error, string, size);
  _last_error[size] = u8'\0';
}

//---
bool Errors::last_error_from_win32()
{
  if (Win32::get_last_error_string(_last_error, 512) == 0) {
    return false;
  }
  return true;
}

//---
bool Errors::to_log(bool show_callstack)
{
  if (show_callstack) {
    if (!Win32::get_callstack(_buffer, MAX_ERROR_LENGTH)) {
      return false;
    }
    ShortStringUnsafe<MAX_ERROR_LENGTH> string(u8"\n************\n", _last_error, u8"\n\n", _buffer, u8"************");
    if (!get_console().set_text_color(get_console().RED) ||
      !get_log().logt(string._str) ||
      !get_console().set_text_color(get_console().WHITE)) {
      return false;
    }
  } else {
    ShortStringUnsafe<MAX_ERROR_LENGTH> string(u8"\n************\n", _last_error, u8"\n", u8"************");
    if (!get_console().set_text_color(get_console().RED) ||
      !get_log().logt(string._str) ||
      !get_console().set_text_color(get_console().WHITE)) {
      return false;
    }
  }
  return true;
}

//---
bool Errors::to_popup()
{
  return true;
}

//
void Errors::kill_script()
{
  if (Memory::_Internal::scripting_mode) {
    ExitProcess(0);
  } else {
    ExitProcess(0);
  }
}
} 