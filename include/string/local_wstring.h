/*
  Documentation: https://www.path.blog/docs/local_wstring.html
*/

#pragma once
#include "../memory/memcpy.h"
#include "../string/strlen.h"
#include "../types.h"

namespace Pathlib::String {

/**/
template <u64 CAPACITY>
struct LocalWString
{
  /**/
  alignas(32) wchar_t str[CAPACITY];
  wchar_t scratch[32];
  u64 length;
  
  /**/
  LocalWString()
  {
    clear();
  }
  ~LocalWString() {}
  
  /**/
  LocalWString(const char* string)
  {
    length = strlen<false, CAPACITY>(string);
    memcpy<true, false>(str, string, length + 1);
  }

  /**/
  inline void clear()
  {
    str[0] = L'\0';
    length = 0;
  }
};
}