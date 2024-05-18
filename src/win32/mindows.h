/*
  !!! DO NOT DISTRIBUTE. This is not safe for scripters to use directly.
  !!! If a function is needed in a header file, wrap it into 'safe_win32.cpp'.
*/

#pragma once

/**/
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

/**/
#define ERROR_IO_PENDING 0x3E5

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

/**/
typedef struct _SECURITY_ATTRIBUTES {
  DWORD  nLength;
  void* lpSecurityDescriptor;
  int   bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

/**/
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

/**/
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
}