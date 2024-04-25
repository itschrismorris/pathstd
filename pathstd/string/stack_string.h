/* 'std/string/string.h'

  + Stack-allocated and bound to CAPACITY length.
  + Utilizes AVX for fast comparisons and copies.
*/

#pragma once
#include "std/memory/avx_memcpy.h"
#include "std/string/avx_strlen.h"
#include "std/types.h"

namespace Path::Std::String {

/**/
template <u64 CAPACITY>
struct String
{
  static_assert(Math::is_multiple(CAPACITY, 32LLU), "String capacity must be a multiple of 32 (AVX-width).");

  /**/
  alignas(32) char str[CAPACITY];
  char scratch[32];
  u32 length;
  
  /**/
  String()
  {
    clear();
  }
  ~String() {}
  
  /**/
  String(const char* string) 
  {
    if (string) { 
      length = Math::min(CAPACITY - 1, avx_strlen<false>(string));
      avx_memcpy<true, false>(str, string, length + 1);
      str[length] = '\0';
    } else {
      clear();
    }
  }

  /**/
  void clear()
  {
    str[0] = '\0';
    length = 0;
  }
};
}