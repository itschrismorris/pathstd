#include "log/log.h"
#include "errors/errors.h"

namespace Pathlib {

/**/
Error Log::initiate(const wchar_t* log_path)
{
  /*
  errno_t _errno = _wfopen_s(&file, log_path, L"w");
  if ((_errno > 0) || (!file)) {
    // TODO: Get string from errno, and add it to our extra error information string.
    return Errors::FILE_OPEN;
  }*/
  return Errors::NONE;
}

/**/
void Log::shutdown()
{
  /*
  if (file) {
    fclose(file);
    file = nullptr;
  }*/
}

/**/
Error Log::log(const wchar_t* message, ...)
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

  return Errors::NONE;
}

/**/
Error Log::error(const wchar_t* message, ...)
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

  return Errors::NONE;
}
}