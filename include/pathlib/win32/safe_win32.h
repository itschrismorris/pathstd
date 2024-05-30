/*
  Documentation: https://www.path.blog/docs/safe_win32.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/string/short_string_unsafe.h"
#include "pathlib/types/string/long_string_unsafe.h"

//---
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
namespace ConsoleColors {

//---
static constexpr u16 FOREGROUND_BLUE = 0x0001;
static constexpr u16 FOREGROUND_GREEN = 0x0002;
static constexpr u16 FOREGROUND_RED = 0x0004;
static constexpr u16 FOREGROUND_INTENSITY = 0x0008;
static constexpr u16 FOREGROUND_WHITE = (FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
static constexpr u16 BACKGROUND_BLUE = 0x0010;
static constexpr u16 BACKGROUND_GREEN = 0x0020;
static constexpr u16 BACKGROUND_RED = 0x0040;
static constexpr u16 BACKGROUND_INTENSITY = 0x0080;
static constexpr u16 BACKGROUND_WHITE = (BACKGROUND_RED | BACKGROUND_GREEN | BACKGROUND_BLUE);
}

//---
static constexpr u32 MAX_PATH_LENGTH = 260;

//---
bool get_callstack(utf8* string_out,
                   u64 string_capacity);
u64 get_last_error_string(utf8* string_out,
                          u64 string_capacity);
bool write_console(const utf8* string,
                   u64 size = Types::U64_MAX);
bool write_log(const utf8* string,
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