#include "error/error.h"
#include "win32/console.h"
#include "../win32/mindows.h"
#include "pathlib.h"

namespace Pathlib {

/**/
Log log;

/**/
bool Log::initiate(const utf8* log_path)
{
  if (log_path) {
    wchar_t utf16_path[MAX_PATH_LENGTH];
    if (Win32::utf8_to_utf16(utf16_path, MAX_PATH_LENGTH, log_path) > 0) {
      file = CreateFileW(utf16_path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
      if ((file == INVALID_HANDLE_VALUE) || (file == nullptr)) {
        file = nullptr;
        error.last_error_from_win32();
        console.set_text_attributes(CONSOLE_FOREGROUND_RED);
        console.write(u8"** Failed to create log file.");
        console.write(error.last_error);
        console.set_text_attributes(CONSOLE_FOREGROUND_WHITE);
        return false;
      }
    } else {
      file = nullptr;
      return false;
    }
  }
  return true;
}

/**/
void Log::shutdown()
{
  if (file) {
    CloseHandle(file);
    file = nullptr;
  }
}
}