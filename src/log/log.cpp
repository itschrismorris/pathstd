#include "log/log.h"
#include "errors/errors.h"

namespace Pathlib {

/**/
Error Log::initiate(const utf8* log_path)
{
  if (log_path) {
    file = CreateFileA(log_path, GENERIC_WRITE, 0, nullptr, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (file == INVALID_HANDLE_VALUE) {
      return Errors::ERROR_FILE_OPEN;
    }
  }
  return Errors::ERROR_NONE;
}

/**/
void Log::shutdown()
{
  if (file) {
    CloseHandle(file);
    file = nullptr;
  }
}

/**/
Error Log::log(const utf8* message, ...)
{
  /*
  if (file) {
    va_list args = nullptr;
    va_start(args, message);
    time_t now = time(nullptr);
    struct tm tm = { 0 };
    errno_t _errno = localtime_s(&tm, &now);
    if (_errno > 0) {
      // TODO: Get string from errno, and add it to our extra error information string.
      return Errors::LOCAL_TIME;
    }
    if ((fwprintf(file, L"%02d.%02d.%02d: ", tm.tm_hour, tm.tm_min, tm.tm_sec) < 0) ||
        (vfwprintf(file, message, args) < 0) ||
        (fwprintf(file, L"\n")) < 0)  {
      return Errors::PRINT_TO_LOG;
    }
    va_end(args);
    fflush(file);
    return Errors::NONE;
  } else {
    return Errors::FILE_HANDLE_NULL;
  }*/

  return Pathlib::Errors::ERROR_NONE;
}

/**/
Error Log::error(const utf8* message, ...)
{
  /*
  if (file) {
    va_list args;
    va_start(args, message);
    time_t now = time(nullptr);
    struct tm tm = { 0 };
    errno_t _errno = localtime_s(&tm, &now);
    if (_errno > 0) {
      // TODO: Get string from errno, and add it to our extra error information string.
      return Errors::LOCAL_TIME;
    }
    if ((fwprintf(file, L"** %02d.%02d.%02d: ", tm.tm_hour, tm.tm_min, tm.tm_sec) < 0) ||
        (vfwprintf(file, message, args) < 0) ||
        (fwprintf(file, L"\n")) < 0) {
      return Errors::PRINT_TO_LOG;
    }
    va_end(args);
    fflush(file);
    return Errors::NONE;
  } else {
    return Errors::FILE_HANDLE_NULL;
  }*/

  return Pathlib::Errors::ERROR_NONE;
}
}