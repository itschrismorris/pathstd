#include "pathlib/pathlib.h"
#include "../src/win32/mindows.h"

namespace Pathlib {

//---
bool pathlib_initiate(const utf8* log_path)
{
  SetConsoleOutputCP(CP_UTF8);
  console.set_text_attributes(Win32::ConsoleColors::GREEN);
  console.write(u8"========================\nInitiating Pathlib.\n========================");
  console.set_text_attributes(Win32::ConsoleColors::WHITE);
  if (!log.initiate(log_path)) {
    return false;
  }
  SystemTime time;
  Win32::get_local_time(&time);
  ShortStringUnsafe<128> time_str(time.wMonth, u8"/", time.wDay, u8"/", time.wYear);
  log.log(time_str);
  if (!profile.initiate() ||
      !timer.initiate()) {
    error.to_log();
    return false;
  }
  Memory::_Internal::scripting_mode = false;
  return true;
}

//---
void pathlib_shutdown()
{
  console.set_text_attributes(Win32::ConsoleColors::GREEN);
  console.write(u8"========================\nShutting down Pathlib.\n========================");
  console.set_text_attributes(Win32::ConsoleColors::WHITE);
  timer.shutdown();
  profile.shutdown();
  log.shutdown();
  console.set_text_attributes(Win32::ConsoleColors::GREEN);
  console.write(u8"========================\nPathlib shutdown.\n========================");
  console.set_text_attributes(Win32::ConsoleColors::WHITE);
}
}