#include "pathlib.h"

namespace Pathlib {

/**/
Error Core::initiate(const utf8* log_path)
{
  SetConsoleOutputCP(CP_UTF8);
  log.initiate(log_path);
  timer.initiate();
  return Errors::ERROR_NONE;
}

/**/
void Core::shutdown()
{
  timer.shutdown();
  log.shutdown();
}
}