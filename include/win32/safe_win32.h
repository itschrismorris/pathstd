/*
  Documentation: https://www.path.blog/docs/safe_win32.html
*/

#pragma once
#include "types.h"
#include "string/local_string.h"

/**/
#define MAX_PATH_LENGTH 260

namespace Pathlib::Win32 {

/**/
u64 get_last_error_string(utf8* string_out,
                          u64 string_capacity);
bool write_console(const utf8* string,
                   u32 size = 0);
u64 utf16_to_utf8(utf8* utf8_string_out,
                  u64 utf8_capacity,
                  const wchar_t* utf16_string,
                  i32 utf16_size = -1);
u64 utf8_to_utf16(wchar_t* utf16_string_out,
                  u64 utf16_capacity,
                  const utf8* utf8_string,
                  i32 utf8_size = -1);
}