/*
  Documentation: https://www.path.blog/docs/profile.html
*/

#pragma once
#include "pathlib/types/types.h"

namespace Pathlib::_Internal {

//---
struct Profile
{
  //---
  bool initiate();
  void shutdown();
};
}

//---
namespace Pathlib { extern _Internal::Profile profile; }