#include "../src/win32/mindows.h"
#include "pathlib/win32/console.h"

namespace Pathlib {

//---
_Internal::Console& get_console()
{
  static _Internal::Console console;
  return console;
}
}

namespace Pathlib::_Internal {

//---
Console::Console()
{
  SetConsoleOutputCP(CP_UTF8);
}

//---
Console::~Console()
{

}

//---
bool Console::set_text_color(u16 color)
{
  return Win32::set_console_text_attributes(color);
}
}