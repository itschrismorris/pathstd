/*
  Documentation: https://www.path.blog/docs/error.html
*/

#pragma once
#include "pathlib/types/types.h"

namespace Pathlib::_Internal {

//---
struct Errors 
{
  //---
  static constexpr u64 MAX_ERROR_LENGTH = 512;

  //---
  utf8 _last_error[MAX_ERROR_LENGTH];
  utf8 _buffer[MAX_ERROR_LENGTH];

  //---
  void set_last_error(const utf8* string);
  bool last_error_from_win32();
  bool to_log(bool show_callstack = true);
  bool to_popup();
  void kill_script();
};
}

//---
namespace Pathlib { _Internal::Errors& get_errors(); }
