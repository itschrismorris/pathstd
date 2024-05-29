/*
  Documentation: https://www.path.blog/docs/errors.html
*/

#pragma once
#include "pathlib/types/types.h"

namespace Pathlib::_Internal {

//---
struct Error {

  //---
  static constexpr u64 MAX_ERROR_LENGTH = 512;

  //---
  utf8 last_error[MAX_ERROR_LENGTH];
  utf8 _buffer[MAX_ERROR_LENGTH];

  //---
  void set_last_error(const utf8* string);
  bool last_error_from_win32();
  bool to_log(bool use_color = true);
  bool to_popup();
  void fatality();
};
}

//---
namespace Pathlib { extern _Internal::Error error; }