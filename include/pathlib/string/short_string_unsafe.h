/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/string/from_type.h"
#include "pathlib/string/compare.h"
#include "pathlib/string/short_string.h"

namespace Pathlib {

//---
template <u64 CAPACITY>
struct ShortStringUnsafe
{
  //---
  alignas(32) utf8 str[CAPACITY];
  u64 size;
  
  //---
  ShortStringUnsafe()
  {
    clear();
  }

  //---
  ShortStringUnsafe(const ShortStringUnsafe& string)
  {
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
  }

  //---
  template <typename... Args>
  ShortStringUnsafe(Args&&... args)
  {
    size = 0;
    (ShortStringUnsafe::_append(*this, args), ...);
  }

  //---
  ~ShortStringUnsafe()
  {
  }

  //---
  inline ShortStringUnsafe& operator =(const ShortStringUnsafe& string)
  {
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  //---
  template <typename T>
  inline ShortStringUnsafe& operator =(const T arg)
  {
    String::_Internal::from_type_clip(arg, str, &size, CAPACITY);
    return *this;
  }

  //---
  inline bool operator ==(const utf8* string) const
  {
    return String::compare<true, false>(str, string, size);
  }

  //---
  inline bool operator ==(const ShortStringUnsafe& string) const
  {
    if (size != string.size) {
      return false;
    }
    return String::compare<true, true>(str, string.str);
  }

  //---
  template <typename T>
  inline const ShortStringUnsafe operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    ShortStringUnsafe new_string = *this;
    ShortStringUnsafe::_append(&new_string, arg);
    return new_string;
  }

  //---
  inline ShortStringUnsafe& operator +=(const ShortString<CAPACITY>& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - size, arg.size);
    memcpy_unsafe<false, true>(&str[size], arg.str, copy_size);
    size += copy_size;
    return *this;
  }

  //---
  inline ShortStringUnsafe& operator +=(const ShortStringUnsafe& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - size, arg.size);
    memcpy_unsafe<false, true>(&str[size], arg.str, copy_size);
    size += copy_size;
    return *this;
  }

  //---
  template <typename T>
  inline ShortStringUnsafe& operator +=(const T arg)
  {
    String::_Internal::from_type_clip(arg, str, &size, CAPACITY);
    return *this;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(ShortStringUnsafe& string_out, 
                             const ShortString<ARG_CAPACITY>& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - string_out.size, arg.size);
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, copy_size + 1);
    string_out.size += copy_size;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(ShortStringUnsafe& string_out, 
                             const ShortStringUnsafe<ARG_CAPACITY>& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - string_out.size, arg.size);
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, copy_size + 1);
    string_out.size += copy_size;
  }

  //---
  template <typename T>
  static inline void _append(ShortStringUnsafe& string_out, 
                             const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        return;
      }
    }
    String::_Internal::from_type_clip(arg, string_out.str, &string_out.size, string_out.get_capacity());
  }

  //---
  template <typename... Args>
  inline void append(Args&&... args)
  {
    (_append(*this, args), ...);
  }

  //---
  template <typename... Args>
  static inline void append(ShortStringUnsafe& string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }
  
  //---
  template <typename... Args>
  inline ShortStringUnsafe& format(Args&&... args)
  {
    size = 0;
    (ShortStringUnsafe::_append(*this, args), ...);
    return *this;
  }

  //---
  template <typename... Args>
  static inline ShortStringUnsafe format(ShortStringUnsafe& string_out,
                                         Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  inline void clear()
  {
    str[0] = u8'\0';
    size = 0;
  }

  //---
  constexpr u64 get_capacity()
  {
    return CAPACITY;
  }

  //---
  static inline u32 hash(const utf8* value)
  {
    return Math::hash(value, String::size_of(value));
  }

  //---
  inline u32 hash() const 
  {
    return Math::hash(str, size);
  }

  //---
  template <typename T>
  ShortStringUnsafe& from_value_hex(T value)
  {
    utf8 chars[] = u8"0123456789ABCDEF";
    constexpr u32 digit_count = sizeof(T) * 2;
    constexpr u32 new_size = digit_count + 2;
    static_assert(new_size < CAPACITY, "ShortString does not have the capacity to hold result of 'from_value_hex()'.");
    str[0] = u8'0';
    str[1] = u8'x';
    #pragma unroll
    for (i32 d = digit_count - 1; d >= 0; --d) {
      str[d + 2] = chars[value & 0xF];
      value >>= 4;
    }
    str[new_size] = u8'\0';
    size = new_size;
    return *this;
  }
};
}

//---
template <typename T> struct _is_short_string : false_type {};
template <u64 CAPACITY> struct _is_short_string<Pathlib::ShortString<CAPACITY>> : true_type {};
template <u64 CAPACITY> struct _is_short_string<Pathlib::ShortStringUnsafe<CAPACITY>> : true_type {};