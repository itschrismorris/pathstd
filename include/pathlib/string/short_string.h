/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/string/from_type.h"
#include "pathlib/string/compare.h"
#include "pathlib/errors/errors.h"

namespace Pathlib {

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
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
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
  operator utf8*() const
  {
    return str;
  }

  //---
  inline ShortString& operator =(const ShortString& string)
  {
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  //---
  template <typename T>
  inline ShortString& operator =(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        get_errors().set_last_error(u8"Attempt to set ShortString to a null pointer.");
        get_errors().to_log();
        get_errors().kill_script();
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
      get_errors().set_last_error(u8"Attempt to compare ShortString equality with a null pointer.");
      get_errors().to_log();
      get_errors().kill_script();
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
    memcpy_unsafe<false, true>(&str[size], arg.str, copy_size);
    size += copy_size;
    return *this;
  }

  //---
  template <typename T>
  inline ShortString& operator +=(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        get_errors().set_last_error(u8"Attempt to append ShortString with a null pointer.");
        get_errors().to_log();
        get_errors().kill_script();
        return *this;
      }
    }
    String::_Internal::from_type_clip(arg, str, &size, CAPACITY);
    return *this;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(ShortString& string_out, 
                             const ShortString<ARG_CAPACITY>& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - string_out.size, arg.get_size());
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.get_str(), copy_size + 1);
    string_out.size += copy_size;
  }

  //---
  template <typename T>
  static inline void _append(ShortString& string_out, 
                             const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        get_errors().set_last_error(u8"Attempt to append ShortString with a null pointer.");
        get_errors().to_log();
        get_errors().kill_script();
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
  inline ShortString& format(Args&&... args)
  {
    size = 0;
    (ShortString::_append(*this, args), ...);
    return *this;
  }

  //---
  template <typename... Args>
  static inline ShortString format(ShortString& string_out,
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
  u64 get_size() const
  {
    return size;
  }

  //---
  const SafePtr<utf8> get_str() const
  {
    SafePtr<utf8> ptr = str;
    ptr.set_count(size);
    return ptr;
  }

  //---
  inline u32 hash() const 
  {
    return Math::hash(str, size);
  }

  //---
  template <typename T>
  ShortString& from_value_hex(T value)
  {
    utf8 chars[] = u8"0123456789ABCDEF";
    constexpr u32 digit_count = sizeof(T) * 2;
    constexpr u32 new_size = digit_count + 2;
    static_assert(new_size < CAPACITY, "ShortString does not have the capacity to hold result of 'from_value_hex()'.");
    str[0] = u8'0';
    str[1] = u8'x';
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