#include "pathlib/memory/arena.h"
#include "pathlib/errors/errors.h"
#include "pathlib/memory/memory.h"
#include "../src/win32/mindows.h"

namespace Pathlib::_Internal {

//---
Arena::Arena()
{
  _head = nullptr;
}

//---
Arena::~Arena()
{
  if (_head) {
    _head = nullptr;
    if (!VirtualFree(_head, 0, MEM_RELEASE)) {
      get_errors().fatal(u8"Failed to free arena.");
      return;
    }
  }
}

//---
bool Arena::allocate(u64 capacity)
{
  SYSTEM_INFO system_info;
  GetSystemInfo(&system_info);
  u64 large_page_size = GetLargePageMinimum();
  u64 page_size = system_info.dwPageSize;
  _capacity = Math::next_multiple_of_pot(capacity, page_size);
  _tail = 0;
  _head = (u8*)VirtualAlloc(nullptr, _capacity,
                            MEM_RESERVE | MEM_COMMIT |
                            MEM_LARGE_PAGES, PAGE_READWRITE);
  if (_head) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_log().logt(u8"Failed to allocate arena with large pages: ", win_err);
    SYSTEM_INFO system_info;
    GetSystemInfo(&system_info);
    _capacity = Math::next_multiple_of_pot(capacity, (u64)system_info.dwPageSize);
  } else {
    get_log().logt(u8"Arena created with large pages (", 
                   large_page_size / Memory::KILOBYTE, 
                   u8" KB), capacity: ", capacity / Memory::KILOBYTE, u8" KB.");
    return true;
  }
  _head = (u8*)VirtualAlloc(nullptr, _capacity,
                            MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
  if (!_head) {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().fatal(FixedStringUnsafe<256>(u8"Failed to allocate arena: ", win_err)._str);
    return false;
  }
  get_log().logt(u8"Arena created without large pages (", 
                 page_size / Memory::KILOBYTE,
                 u8" KB), capacity: ", capacity / Memory::KILOBYTE, u8" KB.");
  return true;
}

//---
void Arena::free()
{
  if (_head) {
    _head = nullptr;
    if (!VirtualFree(_head, 0, MEM_RELEASE)) {
      get_errors().fatal(u8"Failed to free arena.");
      return;
    }
  }
}

//---
void* Arena::push(u64 size)
{
  u8* ptr = &_head[_tail.fetch_add(size, MemOrder::ACQ_REL)];
  if (EXPECT(ptr < (_head + _capacity))) {
    return ptr;
  } else {
    get_errors().fatal(u8"Out of arena memory.");
    return nullptr;
  }
}

//---
void Arena::pop(u64 size)
{
  u64 tail = _tail.fetch_sub(size, MemOrder::ACQ_REL);
  if (tail > _capacity) {
    get_errors().fatal(u8"Over-freed arena.");
  }
}


//---
void Arena::clear()
{
  _tail.store(0, MemOrder::RELEASE);
}
}