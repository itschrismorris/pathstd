#include "../src/win32/mindows.h"
#include "pathlib/win32/safe_win32.h"
#include "pathlib/errors/errors.h"
#include "pathlib/log/log.h"

#pragma comment(lib, "Dbghelp.lib")

namespace Pathlib::Win32 { 

//---
u32 get_current_thread_id()
{
  return GetCurrentThreadId();
}

//---
void yield_thread()
{
  SwitchToThread();
}

//---
void sleep_thread(u32 milliseconds)
{
  Sleep(milliseconds);
}

//---
u64 get_callstack(utf8* string_out,
                  u64 string_capacity)
{
  HANDLE callstack[10];
  HANDLE process = GetCurrentProcess();
  if (!process) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().to_log(u8"Failed to get callstack on error; GetCurrentProcess() failed: ", win_err);
    return false;
  }
  if (!SymInitialize(process, nullptr, true)) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().to_log(u8"Failed to get callstack on error; SymInitialize() failed: ", win_err);
    return false;
  }
  WORD frames = RtlCaptureStackBackTrace(0, 10, callstack, nullptr);
  if (frames == 0) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().to_log(u8"Failed to get callstack on error; RtlCaptureStackBackTrace() failed: ", win_err);
    return false;
  }
  u8 buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
  SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
  symbol->MaxNameLen = MAX_SYM_NAME;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  u64 string_size = 0;
  for (WORD i = 0; i < frames; ++i) {
    DWORD64 address = (DWORD64)(callstack[i]);
    if (!SymFromAddr(process, address, nullptr, symbol)) {
      utf8 win_err[128];
      get_errors().last_error_from_win32(win_err, 128);
      get_errors().to_log(u8"Failed to get callstack on error; SymFromAddr() failed: ", win_err);
      return false;
    }
    if (string_size + symbol->NameLen + 2 < string_capacity) {
      memcpy_unsafe(string_out + string_size, symbol->Name, symbol->NameLen);
      string_size += symbol->NameLen + 1;
      string_out[string_size - 1] = u8'\n';
      string_out[string_size] = u8'\0';
    } else {
      break;
    }
  }
  if (!SymCleanup(process)) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().to_log(u8"Failed to get callstack on error; SymCleanup() failed: ", win_err);
    return 0;
  }
  return string_size;
}

//---
i32 get_last_error()
{
  return GetLastError();
}

//---
u64 get_last_error_string(utf8* string_out,
                          u64 string_capacity)
{
  LPWSTR utf16_string = nullptr;
  DWORD utf16_size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                    nullptr, get_last_error(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&utf16_string, 0, nullptr);
  if (utf16_string && (utf16_size > 0)) {
    u64 utf8_size = utf16_to_utf8(string_out, string_capacity, utf16_string, (u64)utf16_size);
    LocalFree(utf16_string);
    string_out[utf8_size - 2] = u8'\0';
    return utf8_size;
  } else {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().to_log(u8"Failed to get get last win32 error string; FormatMessage() failed: ", win_err);
    string_out[0] = u8'\0';
    return 0;
  }
}

//---
bool write_log(const utf8* string,
               u64 size)
{
  if (get_log()._file) {
    if (WriteFile((HANDLE)get_log()._file, (HANDLE)string, size, nullptr, nullptr) == 0) {
      if (get_last_error() != ERROR_IO_PENDING) {
        utf8 win_err[128];
        get_errors().last_error_from_win32(win_err, 128);
        get_console().write(win_err);
        return false;
      }
    }
    return true;
  }
  return false;
}

//---
bool set_console_text_attributes(u16 attributes)
{
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  if (out) {
    if (!SetConsoleTextAttribute(out, attributes)) {
      return false;
    }
  } else {
    return false;
  }
  return true;
}

//---
bool write_console(const utf8* string,
                   u64 size)
{
  if (size == Types::U64_MAX) {
    size = strlen(string);
  }
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  if (out) {
    wchar_t* utf16_string = (wchar_t*)malloc(size * 2 + 1);
    u64 count = utf8_to_utf16(utf16_string, size * 2 + 1, string, size);
    if (WriteConsoleW(out, utf16_string, count, nullptr, nullptr) == 0) {
      utf8 win_err[128];
      get_errors().last_error_from_win32(win_err, 128);
      get_errors().to_log(u8"Failed to write to console; WriteConsoleW() failed: ", win_err);
      return false;
    }
  } else {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().to_log(u8"Failed to write to console; GetStdHandle() failed: ", win_err);
    return false;
  }
  return true;
}

//---
u64 utf16_to_utf8(utf8* utf8_string_out,
                  u64 utf8_capacity,
                  const wchar_t* utf16_string,
                  i64 utf16_size)
{
  u64 utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_string, utf16_size, nullptr, 0, nullptr, nullptr);
  utf8_size = Math::min(utf8_size, utf8_capacity);
  if (utf8_size > 0) {
    utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_string, utf16_size, (LPSTR)utf8_string_out, utf8_size, nullptr, nullptr);
    if (utf8_size > 0) {
      return (utf8_size - (utf16_size == -1));
    }
  }
  utf8 win_err[128];
  get_errors().last_error_from_win32(win_err, 128);
  get_errors().to_log(u8"Failed to convert utf-16 to utf-8 string: ", win_err);
  utf8_string_out[0] = u8'\0';
  return 0;
}

//---
u64 utf8_to_utf16(wchar_t* utf16_string_out,
                  u64 utf16_capacity,
                  const utf8* utf8_string,
                  i64 utf8_size)
{
  u64 utf16_size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_string, utf8_size, nullptr, 0);
  utf16_size = Math::min(utf16_size, utf16_capacity);
  if (utf16_size > 0) {
    utf16_size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_string, utf8_size, utf16_string_out, utf16_size);
    if (utf16_size > 0) {
      return (utf16_size - (utf8_size == -1));
    }
  }
  utf8 win_err[128];
  get_errors().last_error_from_win32(win_err, 128);
  get_errors().to_log(u8"Failed to convert utf-8 to utf-16 string: ", win_err);
  utf16_string_out[0] = u8'\0';
  return 0;
}

//---
void get_local_time(SystemTime* system_time)
{
  GetLocalTime((LPSYSTEMTIME)system_time);
}
}