#include "errors/errors.h"
#include "core/core.h"

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
void extra_info_from_last_win32_error()
{
  Errors::extra_error_info.size = Win32::get_last_error_string(Errors::extra_error_info.str,
                                                               Errors::extra_error_info.capacity());
}

/**/
void write_last_to_console()
{
  String::LongString string(u8"** ERROR **\n", get_error_string(Errors::last_error_code), u8"\n",
                            Errors::extra_error_info);
  Console::set_text_attributes(CONSOLE_FOREGROUND_RED);
  Win32::write_console(string.str, string.size);
  Console::set_text_attributes(CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_BLUE);
}

/**/
void write_last_to_log()
{
  String::LongString string(u8"** ERROR **\n", get_error_string(Errors::last_error_code), u8"\n",
                            Errors::extra_error_info);
  Console::set_text_attributes(CONSOLE_FOREGROUND_RED);
  LOGT(string.str);
  Console::set_text_attributes(CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_BLUE);
  return;
}

/**/
void write_last_to_popup()
{
  return;
}
} 