/*
  Documentation: https://www.path.blog/docs/thread.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/win32/safe_win32.h"

namespace Pathlib::_Internal {

//---
struct Thread
{
  HANDLE _thread;

};
}