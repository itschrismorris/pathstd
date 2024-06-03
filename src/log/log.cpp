#include "pathlib/errors/errors.h"
#include "pathlib/win32/console.h"
#include "../src/win32/mindows.h"
#include "pathlib/pathlib.h"

namespace Pathlib {

//---
_Internal::Log& get_log()
{
  static _Internal::Log log(PATHLIB_LOG_PATH);
  return log;
}
}

namespace Pathlib::_Internal {

//---
Log::Log(const utf8* log_path) : _buffer(u8"Log::_buffer")
{
  wchar_t utf16_path[Win32::MAX_PATH_LENGTH];
  if (Win32::utf8_to_utf16(utf16_path, Win32::MAX_PATH_LENGTH, log_path) > 0) {
    file = CreateFileW(utf16_path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if ((file == INVALID_HANDLE_VALUE) || (file == nullptr)) {
      file = nullptr;
      get_errors().last_error_from_win32();
      get_console().set_text_color(get_console().RED);
      get_console().write(u8"** Failed to create log file.");
      get_console().write(get_errors().last_error);
      get_console().set_text_color(get_console().WHITE);
    }
  } else {
    file = nullptr;
  }
}

//---
Log::~Log()
{
  if (file) {
    CloseHandle(file);
    file = nullptr;
  }
}
}