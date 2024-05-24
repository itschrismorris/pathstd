/*
  Documentation: https://www.path.blog/docs/safe_win32.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/types/string/long_string.h"

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

//---
static constexpr u32 MAX_PATH_LENGTH = 260;

//---
bool get_callstack(String::LongString<256>* string_out);
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