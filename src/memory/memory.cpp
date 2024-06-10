#include "../src/win32/mindows.h"
#include "pathlib/errors/errors.h"
#include "pathlib/memory/memory.h"

namespace Pathlib {

//---
_Internal::Memory& get_memory()
{
  static _Internal::Memory memory;
  return memory;
}
}

namespace Pathlib::_Internal {

//---
Memory::Memory()
{
  enable_large_pages();
  scoped_arena.allocate(100 * MEGABYTE);
  persistent_arena.allocate(100 * MEGABYTE);
}

//---
Memory::~Memory() 
{
  scoped_arena.free();
  persistent_arena.free();
}

//---
bool Memory::enable_large_pages()
{
  HANDLE token;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &token)) {
    TOKEN_PRIVILEGES privileges = {};
    privileges.PrivilegeCount = 1;
    privileges.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (LookupPrivilegeValueW(0, SE_LOCK_MEMORY_NAME, &privileges.Privileges[0].Luid)) {
      if (AdjustTokenPrivileges(token, FALSE, &privileges, 0, 0, 0)) {
        CloseHandle(token);
        return true;
      } else {
        utf8 win_err[128];
        get_errors().last_error_from_win32(win_err, 128);
        get_errors().to_log(FixedStringUnsafe<256>(u8"Failed to enable large pages; AdjustTokenPrivileges failed: ", win_err)._str);
      }
    } else {
      utf8 win_err[128];
      get_errors().last_error_from_win32(win_err, 128);
      get_errors().to_log(FixedStringUnsafe<256>(u8"Failed to enable large pages; LookupPrivilegeValue failed: ", win_err)._str);
    }
  } else {
    utf8 win_err[128];
    get_errors().last_error_from_win32(win_err, 128);
    get_errors().to_log(FixedStringUnsafe<256>(u8"Failed to enable large pages; OpenProcessToken failed: ", win_err)._str);
  }
  CloseHandle(token);
  return false;
}
}