#pragma once
#include "pathlib/types/types.h"

namespace Pathlib::Memory::_Internal {

//---
struct Arena
{
  //---
  u8* _head;
  u8* _tail;
  u64 size;

  //
  DISALLOW_COPY(Arena);
  explicit Arena(u64 size);
  ~Arena();
};
}