/* 'std/errors.h'

  + A success will return >= 0.
  + Anything < 0 is an error code from the Codes enum, that associates with an error_string[].

    Path: https://www.path.blog
*/

#pragma once
#include "types/types.h"
#include "log/log.h"
#include "types/string/short_string.h"

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
inline String::ShortString<256> extra_error_info;

/**/
i32 get_last_error_code();
const utf8* get_error_string(i32 error_code);
void extra_info_from_last_win32_error();
void write_last_to_console();
void write_last_to_log();
void write_last_to_popup();
}