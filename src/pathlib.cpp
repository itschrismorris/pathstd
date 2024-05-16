#include "pathlib.h"

namespace Pathlib {

/**/
bool Core::initiate(const utf8* log_path)
{
  SetConsoleOutputCP(CP_UTF8);
  if (!log.initiate(log_path) ||
      !timer.initiate()) {
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