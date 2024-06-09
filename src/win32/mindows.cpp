#include "../src/win32/mindows.h"

i32 enable_large_pages()
{
  int Result = false;
  HANDLE TokenHandle;
  if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES, &TokenHandle))
  {
    TOKEN_PRIVILEGES Privs = {};
    Privs.PrivilegeCount = 1;
    Privs.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
    if (LookupPrivilegeValue(0, SE_LOCK_MEMORY_NAME, &Privs.Privileges[0].Luid))
    {
      AdjustTokenPrivileges(TokenHandle, FALSE, &Privs, 0, 0, 0);
      if (GetLastError() == ERROR_SUCCESS)
      {
        Result = true;
      }
      else
      {
        fprintf(stderr, "SeLockMemoryPrivilege denied (user doesn't have group policy set?)\n");
      }
    }
    else
    {
      fprintf(stderr, "LookupPrivilegeValue failed.\n");
    }

    CloseHandle(TokenHandle);
  }
  else
  {
    fprintf(stderr, "OpenProcessToken failed.\n");
  }

  return Result;
}