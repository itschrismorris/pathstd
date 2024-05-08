/* 'std/errors.h'

  + A success will return >= 0.
  + Anything < 0 is an error code from the Codes enum, that associates with an error_string[].

    Path: https://www.path.blog
*/

#pragma once
#include "../types.h"
#include "../log/log.h"
#include "../string/local_wstring.h"

namespace Pathlib::Errors {

/**/
enum Codes
{ 
  NONE = 0,
  UNKNOWN = -1,
  FILE_OPEN = -2,
  FILE_HANDLE_NULL = -3,
  LOCAL_TIME = -4,
  PRINT_TO_LOG = -5,
};

/**/
static String::LocalWString<32> extra;

/**/
static const wchar_t* eng_error_strings[] = 
{ 
  L"Success",
  L"Unknown error",
  L"Error opening file",
  L"File handle was null",
  L"Failed to get localtime",
  L"Failed to print to log file"
};

/**/
bool error(Error error);
}