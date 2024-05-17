/*
  Documentation: https://www.path.blog/docs/short_string.html
*/

#pragma once
#include "string/from_type.h"

namespace Pathlib::String {

/**/
template <u64 CAPACITY>
struct ShortString
{
  static_assert(Math::is_multiple_of<u64, 32>(CAPACITY), "ShortString CAPACITY must be a multiple of 32 (AVX purposes).");

  /**/
  alignas(32) utf8 str[CAPACITY];
  u64 size;
  
  /**/
  ShortString()
  {
    clear();
  }
  ~ShortString() {}

  /**/
  ShortString(const ShortString& string)
  {
    Memory::memcpy<true, true>(str, string.str, string.size);
    size = string.size;
  }

  /**/
  constexpr u64 capacity()
  {
    return CAPACITY;
  }
  
  /**/
  template <typename... Args>
  ShortString(Args&&... args)
  {
    size = 0;
    (ShortString::_append(this, args), ...);
    Memory::memset(&str[size], 0x00, CAPACITY - size);
  }

  /**/
  inline ShortString& operator =(const ShortString& string)
  {
    Memory::memcpy<true, true>(str, string.str, CAPACITY);
    size = string.size;
    return *this;
  }

  /**/
  template <typename T>
  inline ShortString& operator =(const T arg)
  {
    size = String::from_type(arg, str, CAPACITY);
    Memory::memset(&str[size], 0x00, CAPACITY - size);
    return *this;
  }

  /**/
  inline bool operator ==(const ShortString& string) const
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
  template <typename T>
  inline const ShortString operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    ShortString new_string = *this;
    ShortString::_append(&new_string, arg);
    return new_string;
  }

  /**/
  inline ShortString& operator +=(const ShortString& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - size, arg.size);
    Memory::memcpy<false, true>(&str[size], arg.str, copy_size);
    size += copy_size;
    return *this;
  }

  /**/
  template <typename T>
  inline ShortString& operator +=(const T arg)
  {
    size += String::from_type(arg, &str[size], CAPACITY - size);
    return *this;
  }

  /**/
  template <typename... Args>
  inline void format(Args&&... args)
  {
    size = 0;
    (ShortString::_append(this, args), ...);
    Memory::memset(&str[size], 0x00, CAPACITY - size);
  }

  /**/
  inline void clear()
  {
    Memory::memset<true>(str, 0x00, CAPACITY);
    size = 0;
  }

  /**/
  template <typename... Args>
  inline void append(Args&&... args)
  {
    (_append(this, args), ...);
  }

  /**/
  static inline void _append(ShortString* string_out, 
                             const ShortString& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - string_out->size, arg.size);
    Memory::memcpy<false, true>(&string_out->str[string_out->size], arg.str, copy_size);
    string_out->size += copy_size;
  }

  /**/
  template <typename T>
  static inline void _append(ShortString* string_out, 
                             const T arg)
  {
    string_out->size += String::from_type(arg, &string_out->str[string_out->size], CAPACITY - string_out->size);
  }

  /**/
  template <typename... Args>
  static inline void append(ShortString* string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  /**/
  template <typename... Args>
  static inline ShortString format(ShortString* string_out,
                                   Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  /**/
  template <typename... Args>
  static inline ShortString format_copy(Args&&... args)
  {
    ShortString string_out;
    (_append(&string_out, args), ...);
    return string_out;
  }
};
}

/**/
template <typename T> struct _is_short_string : false_type {};
template <u64 CAPACITY> struct _is_short_string<Pathlib::String::ShortString<CAPACITY>> : true_type {};