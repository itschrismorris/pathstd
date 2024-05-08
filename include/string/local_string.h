/* 'string/local_string.h'

  + Recommended for small strings.
  + Bound to CAPACITY length, which must be a multiple of 32 chars for AVX purposes.
  + Allocated "local" to where it is created, so can be stack or heap.
  + Bound to CAPACITY length.
  + Utilizes AVX for faster operations.

    Path: https://www.path.blog
*/

#pragma once
#include "../memory/memcpy.h"
#include "../string/strlen.h"
#include "../types.h"

namespace Pathlib::String {

/**/
template <u64 CAPACITY>
struct LocalString
{
  /**/
  alignas(32) char str[CAPACITY];
  char scratch[32];
  u32 length;
  
  /**/
  LocalString()
  {
    clear();
  }
  ~LocalString() {}
  
  /**/
  LocalString(const char* string)
  {
    length = strlen<false, CAPACITY>(string);
    memcpy<true, false>(str, string, length + 1);
  }

  /**/
  inline void clear()
  {
    str[0] = '\0';
    length = 0;
  }
};
}