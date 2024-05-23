/*
  Documentation: https://www.path.blog/docs/errors.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/string/long_string.h"

namespace Pathlib {

//---
struct Error {

  //---
  String::LongString<> last_error;

  //---
  bool last_error_from_win32();
  bool to_log(bool use_color = true);
  bool to_popup();
};

extern Error error;
}