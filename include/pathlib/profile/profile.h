/*
  Documentation: https://www.path.blog/docs/profile.html
*/

#pragma once
#include "pathlib/types/types.h"

namespace Pathlib {

//---
struct Profile
{
  //---
  bool initiate();
  void shutdown();
};

//---
extern Profile profile;
}