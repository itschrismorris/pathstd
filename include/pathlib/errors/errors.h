/*
  Documentation: https://www.path.blog/docs/error.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/string/long_string_unsafe.h"
#include "pathlib/win32/safe_win32.h"
#include "pathlib/win32/console.h"

namespace Pathlib::_Internal {

//---
struct Errors 
{
  //---
  void last_error_from_win32(utf8* string_out,
                             u64 string_capacity);
  void kill_script();

  //---
  template <typename... Args>
  bool to_log(Args... args)
  {
    LongStringUnsafe<512> buffer(nullptr);
    (buffer._append(buffer, args), ...);
    if (true) {
      LongStringUnsafe<512> message(nullptr, u8"\n************\n", buffer, u8"\n\n");
      message._size += Win32::get_callstack(message._str + message._size, 512 - message._size);
      if (!get_console().set_text_color(get_console().RED) ||
          !get_log().logt(message._str) ||
          !get_console().set_text_color(get_console().WHITE)) {
        return false;
      }
    } else {
      LongStringUnsafe<512> message(nullptr, u8"\n************\n", buffer, u8"\n************");
      if (!get_console().set_text_color(get_console().RED) ||
          !get_log().logt(message._str) ||
          !get_console().set_text_color(get_console().WHITE)) {
        return false;
      }
    }
    return true;
  }
};
}

//---
namespace Pathlib { _Internal::Errors& get_errors(); }
