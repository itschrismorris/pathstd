/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/types/string/from_type.h"
#include "pathlib/types/string/compare.h"
#include "pathlib/error/error.h"

namespace Pathlib::String {

//---
template <u64 CAPACITY>
struct ShortString
{
private:
  //---
  alignas(32) utf8 str[CAPACITY];
  u64 size;

public:
  //---
  ShortString()
  {
    clear();
  }

  //---
  ShortString(const ShortString& string)
  {
    Memory::memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
  }

  //---
  template <typename... Args>
  ShortString(Args&&... args)
  {
    size = 0;
    (_append(*this, args), ...);
  }

  //---
  ~ShortString()
  {
  }

  //---
  constexpr u64 get_capacity()
  {
    return CAPACITY;
  }

  //---
  operator utf8*() const {
    return str;
  }

  //---
  inline ShortString& operator =(const ShortString& string)
  {
    Memory::memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  //---
  template <typename T>
  inline ShortString& operator =(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        error.set_last_error(u8"Attempt to set ShortString to a null pointer.");
        error.to_log();
        error.fatality();
        return false;
      }
    }
    String::_Internal::from_type_clip(arg, str, &size, CAPACITY);
    return *this;
  }

  //---
  inline bool operator ==(const utf8* string) const
  {
    if (EXPECT(string != nullptr)) {
      return String::compare<true, false>(str, string, size);
    } else {
      error.set_last_error(u8"Attempt to compare ShortString equality with a null pointer.");
      error.to_log();
      error.fatality();
      return false;
    }
  }

  //---
  inline bool operator ==(const ShortString& string) const
  {
    if (size != string.size) {
      return false;
    }
    return String::compare<true, true>(str, string.str);
  }

  //---
  template <typename T>
  inline const ShortString operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    ShortString::_append(*this, arg);
    return *this;
  }

  //---
  inline ShortString& operator +=(const ShortString& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - size, arg.size);
    Memory::memcpy_unsafe<false, true>(&str[size], arg.str, copy_size);
    size += copy_size;
    return *this;
  }

  //---
  template <typename T>
  inline ShortString& operator +=(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        error.set_last_error(u8"Attempt to append ShortString with a null pointer.");
        error.to_log();
        error.fatality();
        return *this;
      }
    }
    String::_Internal::from_type_clip(arg, str, &size, CAPACITY);
    return *this;
  }

  //---
  template <typename... Args>
  inline void format(Args&&... args)
  {
    size = 0;
    (ShortString::_append(*this, args), ...);
  }

  //---
  inline void clear()
  {
    str[0] = u8'\0';
    size = 0;
  }

  //---
  static inline void _append(ShortString& string_out, 
                             const ShortString& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - string_out.size, arg.size);
    Memory::memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, copy_size + 1);
    string_out.size += copy_size;
  }

  //---
  template <typename T>
  static inline void _append(ShortString& string_out, 
                             const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        error.set_last_error(u8"Attempt to append ShortString with a null pointer.");
        error.to_log();
        error.fatality();
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
  static inline void append(ShortString& string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  static inline ShortString format(ShortString& string_out,
                                   Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  static inline ShortString format_copy(Args&&... args)
  {
    ShortString string;
    (_append(string, args), ...);
    return string;
  }

  //---
  inline u32 hash() const 
  {
    return Math::hash(str, size);
  }

  //---
  template<u64 _CAPACITY>
  friend struct ShortStringUnsafe;

  //---
  template<u64 _RESERVE_CAPACITY>
  friend struct LongStringUnsafe;

  //---
  template<u64 _RESERVE_CAPACITY>
  friend struct LongString;
};
}