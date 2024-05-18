/*
  Documentation: https://www.path.blog/docs/core.html
*/

#pragma once
#include "log/log.h"
#include "profile/profile.h"
#include "timer/timer.h"

/**/
#define PATHLIB ::Pathlib::pathlib

namespace Pathlib {
  
/**/
struct Core
{
  /**/
  Log log;
  Profile profile;
  Timer timer;

  /**/
  bool initiate(const utf8* log_path = nullptr);
  void shutdown();
};

/**/
inline Core pathlib;
}