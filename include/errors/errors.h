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
  ERROR_UNKNOWN = -1,
  ERROR_INITIALIZING = -2,
  ERROR_FILE_OPEN = -3,
  ERROR_FILE_HANDLE_NULL = -4,
  ERROR_LOCAL_TIME = -5,
  ERROR_PRINT_TO_CONSOLE = -6,
  ERROR_PRINT_TO_LOG = -6,
};

/**/
inline String::LocalString<32> extra;

/**/
inline const utf8* error_strings_eng[] =
{ 
  u8"Success.",
  u8"Unknown error.",
  u8"Error initializing.",
  u8"Error opening file.",
  u8"File handle was null.",
  u8"Failed to get localtime.",
  u8"Failed to print to the console.",
  u8"Failed to print to log file."
};

/**/
bool log_on_error(Error error);
bool popup_on_error(Error error);
}