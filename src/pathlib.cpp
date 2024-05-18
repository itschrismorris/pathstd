#include "pathlib.h"
#include "../src/win32/mindows.h"

namespace Pathlib {

/**/
bool Core::initiate(const utf8* log_path)
{
  SetConsoleOutputCP(CP_UTF8);
  if (!log.initiate(log_path)) {
    return false;
  }
  log.log(u8"Hello world\n");
  if (!timer.initiate()) {
    return false;
  }
  return true;
}

/**/
void Core::shutdown()
{
  timer.shutdown();
  log.shutdown();
}
}