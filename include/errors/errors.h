/* 'std/errors.h'

  + A success will return >= 0.
  + Anything < 0 is an error code from the Codes enum, that associates with an error_string[].

    Path: https://www.path.blog
*/

#pragma once
#include "../types.h"
#include "../log/log.h"
#include "../string/local_string.h"

namespace Pathlib::Errors {

/**/
enum Codes
{ 
  ERROR_NONE = 0,
  ERROR_UNKNOWN,
  ERROR_LOG_CREATE_FILE,
  ERROR_LOG_CONVERTING_PATH,
  ERROR_CONSOLE_WRITE,
};

/**/
inline const utf8* error_strings_eng[] =
{ 
  u8"There was no error.",
  u8"The error is unknown.",
  u8"There was an error creating the log file.",
  u8"There was an error converting the log path from utf-16 to utf-8.",
  u8"There was an error writing to the console."
};

/**/
inline u32 last_error_code;
inline String::LocalString<256> extra_error_info;

/**/
i32 get_last_error_code();
const utf8* get_error_string(i32 error_code);
void write_console_last_error();
void log_last_error();
void popup_last_error();
void extra_info_from_last_win32_error();
}