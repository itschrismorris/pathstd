/*
  Documentation: https://www.path.blog/docs/errors.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/string/long_string.h"

namespace Pathlib::_Internal {

//---
struct Error {

  //---
  String::LongString<256> last_error;
  String::LongString<256> _buffer;

  //---
  bool last_error_from_win32();
  bool to_log(bool use_color = true);
  bool to_popup();
};
}

//---
namespace Pathlib { extern _Internal::Error error; }