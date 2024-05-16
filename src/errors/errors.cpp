#include "win32/safe_win32.h"
#include "errors/errors.h"

namespace Pathlib::Errors {

/**/
i32 get_last_error_code()
{
  return Errors::last_error_code;
}

/**/
const utf8* get_error_string(i32 error_code)
{
  return Errors::error_strings_eng[error_code];
}

/**/
void write_console_last_error()
{
  Win32::write_console(u8"** ERROR **\n", 12);
  Win32::write_console(get_error_string(Errors::last_error_code));
  Win32::write_console(u8"\n", 1);
  Win32::write_console(Errors::extra_error_info.str, Errors::extra_error_info.size);
}

/**/
void log_last_error()
{
  return;
}

/**/
void popup_last_error()
{
  return;
}

/**/
void extra_info_from_last_win32_error()
{
  Errors::extra_error_info.size = Win32::get_last_error_string(Errors::extra_error_info.str,
                                                               Errors::extra_error_info.capacity());
}
} 