#include "errors/errors.h"
#include "win32/console.h"
#include "../src/win32/mindows.h"
#include "pathlib.h"

namespace Pathlib {

/**/
bool Log::initiate(const utf8* log_path)
{
  if (log_path) {
    wchar_t utf16_path[MAX_PATH_LENGTH];
    if (Win32::utf8_to_utf16(utf16_path, MAX_PATH_LENGTH, log_path) > 0) {
      file = CreateFileW(utf16_path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
      if ((file == INVALID_HANDLE_VALUE) || (file == nullptr)) {
        file = nullptr;
        Errors::last_error_code = Errors::ERROR_LOG_CREATE_FILE;
        Errors::extra_info_from_last_win32_error();
        Errors::write_last_to_console();
        return false;
      }
    } else {
      file = nullptr;
      Errors::last_error_code = Errors::ERROR_LOG_CONVERTING_PATH;
      Errors::extra_info_from_last_win32_error();
      Errors::write_last_to_console();
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