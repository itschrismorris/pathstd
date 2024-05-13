/*
  Documentation: https://www.path.blog/docs/safe_win32.html
*/

#pragma once
#include "types.h"

namespace Pathlib::Win32 {

/**/
i32 safe_write_console(const utf8* string,
                       u32 length);
}