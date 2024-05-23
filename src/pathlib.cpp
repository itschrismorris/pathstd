#include "pathlib/pathlib.h"
#include "../src/win32/mindows.h"

namespace Pathlib {

/**/
bool initiate(const utf8* log_path)
{
  SetConsoleOutputCP(CP_UTF8);
  console.set_text_attributes(CONSOLE_FOREGROUND_GREEN);
  console.write(u8"========================\nInitiating Pathlib Core.\n========================");
  console.set_text_attributes(CONSOLE_FOREGROUND_WHITE);
  if (!log.initiate(log_path)) {
    return false;
  }
  SystemTime time;
  Win32::get_local_time(&time);
  String::ShortString<128> time_str(time.wMonth, u8"/", time.wDay, u8"/", time.wYear);
  LOG(time_str);
  if (!profile.initiate() ||
      !timer.initiate()) {
    error.to_log();
    return false;
  }

  return true;
}

/**/
void shutdown()
{
  timer.shutdown();
  profile.shutdown();
  log.shutdown();
  console.set_text_attributes(CONSOLE_FOREGROUND_GREEN);
  console.write(u8"========================\nPathlib Core shutdown.\n========================\n");
  console.set_text_attributes(CONSOLE_FOREGROUND_WHITE);
}
}