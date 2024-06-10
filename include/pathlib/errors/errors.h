/*
  Documentation: https://www.path.blog/docs/error.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/win32/safe_win32.h"
#include "pathlib/win32/console.h"
#include "pathlib/log/log.h"

namespace Pathlib::_Internal {

//---
struct Errors 
{
  //---
  u64 last_error_from_win32(utf8* string_out,
                            u64 string_capacity);
  void fatal(const utf8* error_msg);

  //---
  template <typename... Args>
  bool to_log(Args... args)
  {
    StringUnsafe<256> buffer(MemoryName(u8""), u8"\n************\n");
    (buffer._append(buffer, args), ...);
    buffer.append(u8"\n************");
    if (!get_console().set_text_color(get_console().RED) ||
        !get_log().logt(buffer._str) ||
        !get_console().set_text_color(get_console().WHITE)) {
      return false;
    }
    return true;
  }

  //---
  template <typename... Args>
  bool to_log_with_stacktrace(Args... args)
  {
    StringUnsafe<512> buffer(MemoryName(u8""), u8"\n************\n");
    (buffer._append(buffer, args), ...);
    buffer.append(u8"\n\n");
    buffer._size += Win32::get_callstack(buffer._str + buffer._size, buffer.get_capacity() - buffer._size);
    buffer.append(u8"\n************");
    if (!get_console().set_text_color(get_console().RED) ||
        !get_log().logt(buffer._str) ||
        !get_console().set_text_color(get_console().WHITE)) {
      return false;
    }
    return true;
  }
};
}

//---
namespace Pathlib { _Internal::Errors& get_errors(); }
