#include "../src/win32/mindows.h"
#include "win32/safe_win32.h"
#include "errors/errors.h"

namespace Pathlib::Win32 { 

/**/
i32 get_last_error()
{
  return GetLastError();
}

/**/
u64 get_last_error_string(utf8* string_out,
                          u64 string_capacity)
{
  LPWSTR utf16_string = nullptr;
  u64 utf16_size = FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                                  NULL, get_last_error(), MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPWSTR)&utf16_string, 0, NULL);
  if (utf16_string && (utf16_size > 0)) {
    u64 utf8_size = utf16_to_utf8(string_out, string_capacity, utf16_string, utf16_size);
    LocalFree(utf16_string);
    return utf8_size;
  } else {
    return 0;
  }
}

/**/
bool write_console(const utf8* string,
                   u32 size)
{
  if (size == 0) {
    size = String::size_of(string);
  }
  void* out = GetStdHandle(STD_OUTPUT_HANDLE);
  if (WriteConsoleA(out, string, size, nullptr, nullptr) == 0) {
    Errors::last_error_code = Errors::ERROR_CONSOLE_WRITE;
    Errors::extra_info_from_last_win32_error();
    return false;
  }
  return true;
}

/**/
u64 utf16_to_utf8(utf8* utf8_string_out,
                  u64 utf8_capacity,
                  const wchar_t* utf16_string,
                  i32 utf16_size)
{
  u64 utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_string, utf16_size, NULL, 0, NULL, NULL);
  utf8_size = Math::min(utf8_size, utf8_capacity);
  if (utf8_size > 0) {
    utf8_size = WideCharToMultiByte(CP_UTF8, 0, utf16_string, utf16_size, (LPSTR)utf8_string_out, utf8_size, NULL, NULL);
    if ((utf8_size > 0) && (utf16_size == -1)) {
      return (utf8_size - 1);
    }
  }
  utf8_string_out[utf8_size] = u8'\0';
  return utf8_size;
}

/**/
u64 utf8_to_utf16(wchar_t* utf16_string_out,
                  u64 utf16_capacity,
                  const utf8* utf8_string,
                  i32 utf8_size)
{
  u64 utf16_size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_string, utf8_size, NULL, 0);
  utf16_size = Math::min(utf16_size, utf16_capacity);
  if (utf16_size > 0) {
    utf16_size = MultiByteToWideChar(CP_UTF8, 0, (LPCSTR)utf8_string, utf8_size, utf16_string_out, utf16_size);
    if ((utf16_size > 0) && (utf8_size == -1)) {
      return (utf16_size - 1);
    }
  }
  utf16_string_out[utf16_size] = L'\0';
  return utf16_size;
}
}