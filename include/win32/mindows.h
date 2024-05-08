/* 'mindows.h'

  + Minimal interface to Windows API functions we use.

    Path: https://www.path.blog
*/

#pragma once

/**/
#define STD_OUTPUT_HANDLE  ((unsigned long) - 11)
#define CP_UTF8 65001

extern "C" {

/**/
typedef struct _OVERLAPPED {
  unsigned long Internal;
  unsigned long InternalHigh;
  union {
    struct {
      unsigned long Offset;
      unsigned long OffsetHigh;
    };
    void* Pointer;
  };
  void* hEvent;
} OVERLAPPED, *LPOVERLAPPED;

/**/
__declspec(dllimport) int __stdcall SetConsoleOutputCP(unsigned int wCodePageID);
__declspec(dllimport) void* __stdcall GetStdHandle(unsigned long nStdHandle);
__declspec(dllimport) int __stdcall CloseHandle(void* hObject);
__declspec(dllimport) int __stdcall WriteConsoleA(void*  hConsoleOutput,
                                                  const void* lpBuffer,
                                                  unsigned long nNumberOfCharsToWrite,
                                                  unsigned long* lpNumberOfCharsWritten,
                                                  void* lpReserved);
__declspec(dllimport) int __stdcall WriteFile(void* hFile, const void* lpBuffer, unsigned long nNumberOfBytesToWrite,
                                              unsigned long* lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
}