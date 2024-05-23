#include "../src/win32/mindows.h"
#include "pathlib/win32/safe_win32.h"
#include "pathlib/error/error.h"
#include "pathlib/log/log.h"

#pragma comment(lib, "Dbghelp.lib")

namespace Pathlib::Win32 { 

// ---
bool get_callstack(String::LongString<>* string_out)
{
  HANDLE callstack[10];
  HANDLE process = GetCurrentProcess();
  if (!process) {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  if (!SymInitialize(process, NULL, true)) {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  WORD frames = RtlCaptureStackBackTrace(0, 10, callstack, NULL);
  if (frames == 0) {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  u8 buffer[sizeof(SYMBOL_INFO) + MAX_SYM_NAME * sizeof(TCHAR)];
  SYMBOL_INFO* symbol = (SYMBOL_INFO*)buffer;
  symbol->MaxNameLen = MAX_SYM_NAME;
  symbol->SizeOfStruct = sizeof(SYMBOL_INFO);
  for (WORD i = 0; i < frames; ++i) {
    DWORD64 address = (DWORD64)(callstack[i]);
    if (!SymFromAddr(process, address, NULL, symbol)) {
      error.last_error_from_win32();
      error.to_log();
      return false;
    }
    *string_out += (utf8*)symbol->Name;
    *string_out += u8'\n';
  }
  if (!SymCleanup(process)) {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  return true;
}

// ---
i32 get_last_error()
{
  return GetLastError();
}

// ---
u64 get_last_error_string(utf8* string_out,
                          u64 string_capacity)
{
  LPWSTR utf16_string = nullptr;
  u64 utf16_size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                  NULL, get_last_error(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&utf16_string, 0, NULL);
  if (utf16_string && (utf16_size > 0)) {
    u64 utf8_size = utf16_to_utf8(string_out, string_capacity, utf16_string, utf16_size);
    LocalFree(utf16_string);
    string_out[utf8_size] = u8'\0';
    return utf8_size;
  } else {
    error.last_error_from_win32();
    error.to_log();
    string_out[0] = u8'\0';
    return 0;
  }
}

// ---
bool write_log(const utf8* string,
               u64 size)
{
  if (log.file) {
    if (WriteFile((HANDLE)log.file, (HANDLE)string, size, nullptr, nullptr) == 0) {
      if (get_last_error() != ERROR_IO_PENDING) {
        error.last_error_from_win32();
        error.to_log();
        return false;
      }
    }
    return true;
  } else {
    error.last_error = u8"File handle for write_file() was null.";
    error.to_log();
  }
  return false;
}

// ---
bool set_console_text_attributes(u16 attributes)
{
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  if (out) {
    if (!SetConsoleTextAttribute(out, attributes)) {
      error.last_error_from_win32();
      error.to_log();
      return false;
    }
  } else {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  return true;
}

// ---
bool write_console(const utf8* string,
                   u64 size)
{
  if (size == U64_MAX) {
    size = String::size_of(string);
  }
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  if (out) {
    if (WriteConsoleA(out, string, size, nullptr, nullptr) == 0) {
      error.last_error_from_win32();
      error.to_log();
      return false;
    }
  } else {
    error.last_error_from_win32();
    error.to_log();
    return false;
  }
  return true;
}

// ---
u64 utf16_to_utf8(utf8* utf8_string_out,
                  u64 utf8_capacity,
                  const wchar_t* utf16_string,
                  i64 utf16_size)
{
  u64 utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_string, utf16_size, NULL, 0, NULL, NULL);
  utf8_size = Math::min(utf8_size, utf8_capacity);
  if (utf8_size > 0) {
    utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_string, utf16_size, (LPSTR)utf8_string_out, utf8_size, NULL, NULL);
    if (utf8_size > 0) {
      return (utf8_size - (utf16_size == -1));
    }
  }
  error.last_error_from_win32();
  error.to_log();
  utf8_string_out[0] = u8'\0';
  return 0;
}

// ---
u64 utf8_to_utf16(wchar_t* utf16_string_out,
                  u64 utf16_capacity,
                  const utf8* utf8_string,
                  i64 utf8_size)
{
  u64 utf16_size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_string, utf8_size, NULL, 0);
  utf16_size = Math::min(utf16_size, utf16_capacity);
  if (utf16_size > 0) {
    utf16_size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_string, utf8_size, utf16_string_out, utf16_size);
    if (utf16_size > 0) {
      return (utf16_size - (utf8_size == -1));
    }
  }
  error.last_error_from_win32();
  error.to_log();
  utf16_string_out[0] = u8'\0';
  return 0;
}

// ---
void get_local_time(SystemTime* system_time)
{
  GetLocalTime((LPSYSTEMTIME)system_time);
}
}