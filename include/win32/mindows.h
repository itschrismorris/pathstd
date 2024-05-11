/*
  Documentation: https://www.path.blog/docs/mindows.html
*/

#pragma once

/**/
#define STD_OUTPUT_HANDLE  ((unsigned long) - 11)
#define CP_UTF8 65001
#define CREATE_ALWAYS 2
#define FILE_ATTRIBUTE_NORMAL 0x80
#define GENERIC_WRITE 0x40000000L
#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

/**/
typedef __int64 LONG_PTR;
typedef void* LPVOID;
typedef void* PVOID;
typedef long LONG;
typedef long long LONGLONG;
typedef const void* LPCVOID;
typedef unsigned short WORD;
typedef unsigned char BYTE;
typedef char CHAR;
typedef int BOOL;
typedef unsigned long DWORD;
typedef char* LPSTR, *PSTR;
typedef const char* LPCSTR, *PCSTR;
typedef PVOID HANDLE;
typedef HANDLE HMODULE;
typedef HANDLE HINSTANCE;
typedef HANDLE HWND;
typedef LONG HRESULT;
typedef unsigned int UINT;

extern "C" {

/**/
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

typedef struct _SECURITY_ATTRIBUTES {
  DWORD  nLength;
  void* lpSecurityDescriptor;
  int   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

/**/
__declspec(dllimport) int __stdcall SetConsoleOutputCP(UINT wCodePageID);
__declspec(dllimport) HANDLE __stdcall GetStdHandle(DWORD nStdHandle);
__declspec(dllimport) int __stdcall CloseHandle(HANDLE hObject);
__declspec(dllimport) int __stdcall WriteConsoleA(HANDLE  hConsoleOutput,
                                                  const HANDLE lpBuffer,
                                                  DWORD nNumberOfCharsToWrite,
                                                  DWORD* lpNumberOfCharsWritten,
                                                  HANDLE lpReserved);
__declspec(dllimport) HANDLE __stdcall CreateFileA(const char8_t* lpFileName, 
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
}