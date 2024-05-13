/*
  Documentation: https://www.path.blog/docs/pathlib.html
*/

#pragma once
#include "log/log.h"
#include "errors/errors.h"
#include "win32/console.h"

using namespace Pathlib::String;
using namespace Pathlib::Win32;
using namespace Pathlib::Errors;

namespace Pathlib {
  
/**/
struct Core
{
  /**/
  Pathlib::Log log;

  /**/
  Error initiate(const utf8* log_path = nullptr);
  void shutdown();
};

/**/
inline Core core;
}