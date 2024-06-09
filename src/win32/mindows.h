/*
  !!! DO NOT DISTRIBUTE. This is not safe for scripters to use directly.
  !!! If a function is needed in a header file, wrap it into 'safe_win32.cpp'.
*/

#pragma once
#include "pathlib/types/types.h"

//---
#define STD_OUTPUT_HANDLE  ((unsigned long) - 11)
#define CP_UTF8 65001
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
#define GENERIC_WRITE 0x40000000L
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)
#define FORMAT_MESSAGE_ALLOCATE_BUFFER 0x00000100
#define FORMAT_MESSAGE_FROM_SYSTEM  0x00001000
#define FORMAT_MESSAGE_IGNORE_INSERTS 0x00000200
#define LANG_NEUTRAL 0x00
#define SUBLANG_DEFAULT 0x01
#define MAKELANGID(p, s) ((((WORD)(s)) << 10) | (WORD)(p))
#define MAX_SYM_NAME 2000
#define INFINITE 0xFFFFFFFF
#define WAIT_TIMEOUT 0x00000102L
#define WAIT_OBJECT_0 0x00000000L
#define MEM_COMMIT 0x00001000
#define MEM_RESERVE 0x00002000
#define MEM_LARGE_PAGES 0x20000000
#define PAGE_READWRITE 0x04
#define MEM_RELEASE 0x00008000

//---
#define ERROR_IO_PENDING 0x3E5

//---
typedef unsigned __int64 SIZE_T;
typedef __int64 LONG_PTR;
typedef void* LPVOID;
typedef void* PVOID;
typedef long LONG;
typedef unsigned short USHORT;
typedef unsigned long ULONG;
typedef unsigned long long ULONG64;
typedef unsigned long* PULONG;
typedef long long LONGLONG;
typedef const void* LPCVOID;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef char CHAR;
typedef int BOOL;
typedef BOOL* LPBOOL;
typedef unsigned long DWORD;
typedef char* LPSTR, *PSTR;
typedef wchar_t* LPWSTR;
typedef const wchar_t* LPCWSTR;
typedef const char* LPCSTR, *PCSTR;
typedef PVOID HANDLE;
typedef HANDLE HMODULE;
typedef HANDLE HINSTANCE;
typedef HANDLE HWND;
typedef LONG HRESULT;
typedef unsigned int UINT;
typedef HANDLE HLOCAL;
typedef wchar_t TCHAR;
typedef unsigned __int64 DWORD64;
typedef DWORD64* PDWORD64;
typedef BOOL(__stdcall* PHANDLER_ROUTINE)(DWORD dwCtrlType);
typedef DWORD* LPDWORD;
typedef DWORD(__stdcall* LPTHREAD_START_ROUTINE)(LPVOID lpParameter);

extern "C" {

//---
typedef union _LARGE_INTEGER {
  struct {
    DWORD LowPart;
    LONG HighPart;
  } DUMMYSTRUCTNAME;
  struct {
    DWORD LowPart;
    LONG HighPart;
  } u;
  LONGLONG QuadPart;
} LARGE_INTEGER;

//---
typedef struct _SECURITY_ATTRIBUTES {
  DWORD  nLength;
  void* lpSecurityDescriptor;
  int   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

//---
typedef struct _SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
} SYSTEMTIME, *PSYSTEMTIME, *LPSYSTEMTIME;

//---
typedef struct _SYMBOL_INFO {
  ULONG       SizeOfStruct;
  ULONG       TypeIndex;
  ULONG64     Reserved[2];
  ULONG       Index;
  ULONG       Size;
  ULONG64     ModBase;
  ULONG       Flags;
  ULONG64     Value;
  ULONG64     Address;
  ULONG       Register;
  ULONG       Scope;
  ULONG       Tag;
  ULONG       NameLen;
  ULONG       MaxNameLen;
  CHAR        Name[1];
} SYMBOL_INFO, *PSYMBOL_INFO;

i32 enable_large_pages();

//---
__declspec(dllimport) HANDLE __stdcall GetCurrentProcess();
__declspec(dllimport) LPVOID __stdcall VirtualAlloc(LPVOID lpAddress,
                                                    SIZE_T dwSize,
                                                    DWORD  flAllocationType,
                                                    DWORD  flProtect);
__declspec(dllimport) BOOL __stdcall VirtualFree(LPVOID lpAddress,
                                                 SIZE_T dwSize,
                                                 DWORD  dwFreeType);
__declspec(dllimport) SIZE_T __stdcall GetLargePageMinimum();
__declspec(dllimport) BOOL __stdcall AdjustTokenPrivileges(HANDLE TokenHandle,
                                                           BOOL DisableAllPrivileges,
                                                           PTOKEN_PRIVILEGES NewState,
                                                           DWORD BufferLength,
                                                           PTOKEN_PRIVILEGES PreviousState,
                                                           PDWORD ReturnLength);
__declspec(dllimport) DWORD __stdcall GetCurrentThreadId();
__declspec(dllimport) BOOL __stdcall SwitchToThread();
__declspec(dllimport) BOOL __stdcall GetExitCodeThread(HANDLE  hThread,
                                                       LPDWORD lpExitCode);
__declspec(dllimport) HANDLE __stdcall CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes,
                                                    SIZE_T dwStackSize,
                                                    LPTHREAD_START_ROUTINE lpStartAddress,
                                                    LPVOID lpParameter,
                                                    DWORD dwCreationFlags,
                                                    LPDWORD lpThreadId);
__declspec(dllimport) BOOL __stdcall TerminateThread(HANDLE hThread,
                                                     DWORD dwExitCode);
__declspec(dllimport) DWORD WaitForSingleObject(HANDLE hHandle,
                                                DWORD dwMilliseconds);
__declspec(dllimport) void __stdcall Sleep(DWORD dwMilliseconds);
__declspec(dllimport) BOOL __stdcall SymInitialize(HANDLE hProcess,
                                                   PCSTR UserSearchPath,
                                                   BOOL fInvadeProcess);
__declspec(dllimport) BOOL __stdcall SymCleanup(HANDLE hProcess);
__declspec(dllimport) USHORT __stdcall RtlCaptureStackBackTrace(ULONG FramesToSkip,
                                                             ULONG FramesToCapture,
                                                             PVOID* BackTrace,
                                                             PULONG BackTraceHash);
__declspec(dllimport) BOOL __stdcall SymFromAddr(HANDLE hProcess,
                                                 DWORD64 Address,
                                                 PDWORD64 Displacement,
                                                 PSYMBOL_INFO Symbol);
__declspec(dllimport) int __stdcall SetConsoleOutputCP(UINT wCodePageID);
__declspec(dllimport) HANDLE __stdcall GetStdHandle(DWORD nStdHandle);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE hObject);
__declspec(dllimport) int __stdcall WriteConsoleA(HANDLE  hConsoleOutput,
                                                  const void* lpBuffer,
                                                  DWORD nNumberOfCharsToWrite,
                                                  DWORD* lpNumberOfCharsWritten,
                                                  HANDLE lpReserved);
__declspec(dllimport) HANDLE __stdcall CreateFileW(const wchar_t* lpFileName, 
                                                   DWORD dwDesiredAccess,
                                                   DWORD dwShareMode,
                                                   LPSECURITY_ATTRIBUTES lpSecurityAttributes,
                                                   DWORD dwCreationDisposition,
                                                   DWORD dwFlagsAndAttributes,
                                                   HANDLE hTemplateFile);
__declspec(dllimport) int __stdcall WriteFile(HANDLE hFile, const HANDLE lpBuffer, DWORD nNumberOfBytesToWrite,
                                              DWORD* lpNumberOfBytesWritten, void* ignore);
__declspec(dllimport) int __stdcall QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);
__declspec(dllimport) int __stdcall QueryPerformanceCounter(LARGE_INTEGER *lpPerformanceCount);
__declspec(dllimport) DWORD __stdcall GetLastError();
__declspec(dllimport) DWORD __stdcall FormatMessageW(DWORD dwFlags,
                                                     LPCVOID lpSource,
                                                     DWORD dwMessageId,
                                                     DWORD dwLanguageId,
                                                     LPWSTR lpBuffer,
                                                     DWORD nSize,
                                                     void* ignore);
__declspec(dllimport) int __stdcall WideCharToMultiByte(UINT CodePage,
                                                        DWORD  dwFlags,
                                                        LPCWSTR lpWideCharStr,
                                                        int cchWideChar,
                                                        LPSTR lpMultiByteStr,
                                                        int cbMultiByte,
                                                        LPCSTR lpDefaultChar,
                                                        LPBOOL lpUsedDefaultChar);
__declspec(dllimport) int __stdcall MultiByteToWideChar(UINT CodePage,
                                                        DWORD dwFlags,
                                                        LPCSTR lpMultiByteStr,
                                                        int cbMultiByte,
                                                        LPWSTR lpWideCharStr,
                                                        int cchWideChar);
__declspec(dllimport) void __stdcall LocalFree(HLOCAL hMem);
__declspec(dllimport) void __stdcall GetLocalTime(LPSYSTEMTIME lpSystemTime);
__declspec(dllimport) BOOL __stdcall SetConsoleTextAttribute(HANDLE hConsoleOutput,
                                                             WORD wAttributes);
__declspec(dllimport) BOOL __stdcall SetConsoleCtrlHandler(PHANDLER_ROUTINE HandlerRoutine,
                                                           BOOL Add);
__declspec(dllimport) void __stdcall ExitProcess(UINT uExitCode);
}