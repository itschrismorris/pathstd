#include "errors/errors.h"
#include "../src/win32/mindows.h"
#include "win32/console.h"
#include "core/core.h"

namespace Pathlib {

/**/
bool Core::initiate(const utf8* log_path)
{
  SetConsoleOutputCP(CP_UTF8);
  Console::set_text_attributes(CONSOLE_FOREGROUND_GREEN);
  Console::write(u8"\n========================\nInitiating Pathlib Core.\n========================");
  Console::set_text_attributes(CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_BLUE);
  if (!log.initiate(log_path)) {
    return false;
  }
  SystemTime time;
  Win32::get_local_time(&time);
  String::ShortString<128> time_str(time.wMonth, u8"/", time.wDay, u8"/", time.wYear);
  LOG(time_str);
  if (!profile.initiate() ||
      !timer.initiate()) {
    Errors::write_last_to_log();
    return false;
  }
  return true;
}

/**/
void Core::shutdown()
{
  timer.shutdown();
  profile.shutdown();
  log.shutdown();
  Console::set_text_attributes(CONSOLE_FOREGROUND_GREEN);
  Console::write(u8"========================\nPathlib Core shutdown.\n========================\n");
  Console::set_text_attributes(CONSOLE_FOREGROUND_RED | CONSOLE_FOREGROUND_GREEN | CONSOLE_FOREGROUND_BLUE);
}
}