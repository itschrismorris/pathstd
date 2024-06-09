#include "pathlib/memory/arena.h"
#include "pathlib/errors/errors.h"
#include "../src/win32/mindows.h"

namespace Pathlib::Memory::_Internal {

Arena::Arena(u64 size)
{
  u64 large_page_minimum = GetLargePageMinimum();
  size = Math::next_multiple_of_pot(size, large_page_minimum);
  _head = _tail = (u8*)VirtualAlloc(nullptr, size,
                                    MEM_RESERVE | MEM_COMMIT | 
                                    MEM_LARGE_PAGES, PAGE_READWRITE);
  if (!_head) {
    utf8 win_err[256];
    get_errors().last_error_from_win32(win_err, 256);
    get_errors().fatal(FixedStringUnsafe<256>(u8"Failed to allocate arena: ", win_err)._str);
    return;
  }
}

//---
Arena::~Arena()
{
  if (!VirtualFree(_head, 0, MEM_RELEASE)) {
    get_errors().fatal(u8"Failed to free arena.");
    return;
  }
}
}