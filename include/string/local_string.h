/*
  Documentation: https://www.path.blog/docs/local_string.html
*/

#pragma once
#include "string/from_type.h"

namespace Pathlib::String {

/**/
template <u64 CAPACITY>
struct LocalString
{
  static_assert(Math::is_multiple_of<u64, 32>(CAPACITY), "LocalString CAPACITY must be a multiple of 32 (AVX purposes).");

  /**/
  alignas(32) utf8 str[CAPACITY];
  u64 size;
  
  /**/
  LocalString()
  {
    clear();
  }
  ~LocalString() {}

  /**/
  LocalString(const LocalString& string)
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
  LocalString(Args&&... args)
  {
    size = 0;
    (LocalString::_append(this, args), ...);
    Memory::memset(&str[size], 0x00, CAPACITY - size);
  }

  /**/
  inline LocalString& operator =(const LocalString& string)
  {
    Memory::memcpy<true, true>(str, string.str, CAPACITY);
    size = string.size;
    return *this;
  }

  /**/
  template <typename T>
  inline LocalString& operator =(const T arg)
  {
    size += String::from_type(arg, str, CAPACITY);
    Memory::memset(&str[size], 0x00, CAPACITY - size);
    return *this;
  }

  /**/
  inline bool operator ==(const LocalString& string) const
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
  inline const LocalString operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    LocalString new_string = *this;
    LocalString::_append(&new_string, arg);
    return new_string;
  }

  /**/
  inline LocalString& operator +=(const LocalString& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - size, arg.size);
    Memory::memcpy<false, true>(&str[size], arg.str, copy_size);
    size += copy_size;
    return *this;
  }

  /**/
  template <typename T>
  inline LocalString& operator +=(const T arg)
  {
    size += String::from_type(arg, &str[size], CAPACITY - size);
    return *this;
  }

  /**/
  template <typename... Args>
  inline void format(Args&&... args)
  {
    size = 0;
    (LocalString::_append(this, args), ...);
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
  static inline void _append(LocalString* string_out, 
                             const LocalString& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - string_out->size, arg.size);
    Memory::memcpy<false, true>(&string_out->str[string_out->size], arg.str, copy_size);
    string_out->size += copy_size;
  }

  /**/
  template <typename T>
  static inline void _append(LocalString* string_out, 
                             const T arg)
  {
    string_out->size += String::from_type(arg, &string_out->str[string_out->size], CAPACITY - string_out->size);
  }

  /**/
  template <typename... Args>
  static inline void append(LocalString* string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  /**/
  template <typename... Args>
  static inline LocalString format(LocalString* string_out,
                                   Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  /**/
  template <typename... Args>
  static inline LocalString format_copy(Args&&... args)
  {
    LocalString string_out;
    (_append(&string_out, args), ...);
    return string_out;
  }
};
}

/**/
template <typename T> struct _is_local_string : false_type {};
template <u64 CAPACITY> struct _is_local_string<Pathlib::String::LocalString<CAPACITY>> : true_type {};