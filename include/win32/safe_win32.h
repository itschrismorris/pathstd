/*
  Documentation: https://www.path.blog/docs/safe_win32.html
*/

#pragma once
#include "types/types.h"
#include "types/string/short_string.h"
#include "types/string/long_string.h"

/**/
#define MAX_PATH_LENGTH 260
#define CONSOLE_FOREGROUND_BLUE 0x0001
#define CONSOLE_FOREGROUND_GREEN 0x0002
#define CONSOLE_FOREGROUND_RED 0x0004
#define CONSOLE_FOREGROUND_INTENSITY 0x0008
#define CONSOLE_FOREGROUND_WHITE (CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_BLUE)
#define CONSOLE_BACKGROUND_BLUE 0x0010
#define CONSOLE_BACKGROUND_GREEN 0x0020
#define CONSOLE_BACKGROUND_RED 0x0040
#define CONSOLE_BACKGROUND_INTENSITY 0x0080
#define CONSOLE_BACKGROUND_WHITE (CONSOLE_BACKGROUND_RED | CONSOLE_BACKGROUND_GREEN | CONSOLE_BACKGROUND_BLUE)

/**/
struct SystemTime 
{
  u16 wYear;
  u16 wMonth;
  u16 wDayOfWeek;
  u16 wDay;
  u16 wHour;
  u16 wMinute;
  u16 wSecond;
  u16 wMilliseconds;
};

namespace Pathlib::Win32 {

/**/
bool get_callstack(String::LongString<>* string_out);
u64 get_last_error_string(utf8* string_out,
                          u64 string_capacity);
bool write_console(const utf8* string,
                   u64 size = U64_MAX);
bool write_file(void* file,
                const utf8* string,
                u64 size);
bool set_console_text_attributes(u16 attributes);
u64 utf16_to_utf8(utf8* utf8_string_out,
                  u64 utf8_capacity,
                  const wchar_t* utf16_string,
                  i64 utf16_size = -1);
u64 utf8_to_utf16(wchar_t* utf16_string_out,
                  u64 utf16_capacity,
                  const utf8* utf8_string,
                  i64 utf8_size = -1);
void get_local_time(SystemTime* system_time);
}