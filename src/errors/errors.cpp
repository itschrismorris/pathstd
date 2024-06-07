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
void Errors::last_error_from_win32(utf8* string_out,
                                   u64 string_capacity)
{
  Win32::get_last_error_string(string_out, string_capacity);
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