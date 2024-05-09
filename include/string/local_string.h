/*
  Documentation: https://www.path.blog/docs/local_wstring.html
*/

#pragma once
#include "../memory/memcpy.h"
#include "../memory/memset.h"
#include "../string/strlen.h"
#include "../string/from_float.h"
#include "../string/from_int.h"
#include "../types.h"

namespace Pathlib::String {

/**/
template <u64 CAPACITY>
struct LocalString
{
  static_assert(Math::is_multiple_of<32>(CAPACITY), "LocalString CAPACITY must be a multiple of 32 (AVX purposes).");

  /**/
  alignas(32) utf8 str[CAPACITY];
  u64 length;
  
  /**/
  LocalString()
  {
    clear();
  }
  ~LocalString() {}
  
  /**/
  LocalString(const utf8* string)
  {
    length = Math::min(CAPACITY - 1, String::strlen(string));
    Memory::memcpy<true, false>(str, string, length);
    Memory::memset(&str[length], 0x00, CAPACITY - length);
  }

  /**/
  LocalString& operator =(const utf8* string)
  {
    length = Math::min(CAPACITY - 1, String::strlen(string));
    Memory::memcpy<true, false>(str, string, length);
    Memory::memset(&str[length], 0x00, CAPACITY - length);
    return *this;
  }

  /**/
  LocalString& operator =(LocalString& string)
  {
    length = Math::min(CAPACITY - 1, string.length);
    Memory::memcpy<true, false>(str, string.str, length);
    Memory::memset(&str[length], 0x00, CAPACITY - length);
    return *this;
  }

  /**/
  LocalString& operator =(LocalString string)
  {
    length = Math::min(CAPACITY - 1, string.length);
    Memory::memcpy<true, false>(str, string.str, length);
    Memory::memset(&str[length], 0x00, CAPACITY - length);
    return *this;
  }

  /**/
  bool operator ==(const LocalString& string) const
  {
    u32 bitmask = U32_MAX;
    #pragma unroll
    for (u32 c = 0; c < (CAPACITY >> 5); ++c) {
      I8 ours = I8_LOAD(&str[c * 32]);
      I8 theirs = I8_LOAD(&string.str[c * 32]);
      bitmask &= I8_MOVEMASK(I8_CMP_EQ(ours, theirs));
    }
    return (bitmask == U32_MAX);
  }

  /**/
  LocalString operator +(const utf8* string)
  {
    LocalString<CAPACITY> s;
    u64 copy_length = Math::min(CAPACITY - 1, length);
    Memory::memcpy<true, true>(s.str, str, copy_length);
    s.length = copy_length;
    copy_length = Math::min((CAPACITY - 1) - s.length, String::strlen(string));
    Memory::memcpy(&s.str[s.length], string, copy_length);
    s.length += copy_length;
    Memory::memset(&s.str[s.length], 0x00, CAPACITY - s.length);
    return s;
  }

  /**/
  LocalString operator +(const LocalString& string)
  {
    LocalString<CAPACITY> s;
    u64 copy_length = Math::min(CAPACITY - 1, length);
    Memory::memcpy<true, true>(s.str, str, copy_length);
    s.length = copy_length;
    copy_length = Math::min((CAPACITY - 1) - s.length, string.length);
    Memory::memcpy<false, true>(&s.str[s.length], string.str, copy_length);
    s.length += copy_length;
    Memory::memset(&s.str[s.length], 0x00, CAPACITY - s.length);
    return s;
  }

  /**/
  LocalString& operator +=(const utf8* string)
  {
    u64 copy_length = Math::min((CAPACITY - 1) - length, String::strlen(string));
    Memory::memcpy(&str[length], string, copy_length);
    length += copy_length;
    Memory::memset(&str[length], 0x00, CAPACITY - length);
    return *this;
  }

  /**/
  LocalString& operator +=(const LocalString& string)
  {
    u64 copy_length = Math::min((CAPACITY - 1) - length, string.length);
    Memory::memcpy<false, true>(&str[length], string.str, copy_length);
    length += copy_length;
    Memory::memset(&str[length], 0x00, CAPACITY - length);
    return *this;
  }

  /**/
  template <typename T>
  static inline LocalString to_string(T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
      return arg;
    } else if constexpr (SAME_TYPE(T, i32) || SAME_TYPE(T, i64) || 
                         SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
      utf8 buffer[32];
      return String::from_int(arg, buffer);
    } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
      utf8 buffer[32];
      return String::from_float(arg, buffer);
    }
  }

  /**/
  template <typename... Args>
  static inline LocalString format(Args&&... args)
  {
    return (... + to_string(args));
  }

  /**/
  inline void clear()
  {
    Memory::memset<true>(str, 0x00, CAPACITY);
    length = 0;
  }
};
}