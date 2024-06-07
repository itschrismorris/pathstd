/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/string/from_type.h"
#include "pathlib/string/compare.h"

namespace Pathlib {

//---
template <u64 CAPACITY>
struct ShortStringUnsafe
{
  //---
  alignas(32) utf8 _str[CAPACITY];
  u64 _size;
  
  //---
  template <typename... Args>
  ShortStringUnsafe(Args&&... args)
  {
    clear();
    (ShortStringUnsafe::_append(*this, args), ...);
  }

  //---
  ShortStringUnsafe(const ShortStringUnsafe& string)
  {
    memcpy_unsafe<true, true>(_str, string._str, string._size + 1);
    _size = string._size;
  }

  //---
  ~ShortStringUnsafe() {}

  //---
  template <typename T>
  inline ShortStringUnsafe& operator =(const T& arg)
  {
    _size = 0;
    _append(*this, arg);
    return *this;
  }

  //---
  template <typename T>
  inline bool operator ==(T& string)
  {
    if constexpr (IS_UNSAFE_LONG_STRING(T) || IS_UNSAFE_SHORT_STRING(T)) {
      return String::compare<true, true>(_str, string._str, _size, string._size);
    } else if constexpr (IS_SAFE_LONG_STRING(T) || IS_SAFE_SHORT_STRING(T)) {
      return String::compare<true, true>(_str, string.get_str(), _size, string.get_size());
    } else if constexpr (SAME_TYPE(ARRAY_TYPE(T), const utf8) || SAME_TYPE(ARRAY_TYPE(T), utf8) || 
                         SAME_TYPE(T&, const utf8*&) || SAME_TYPE(T&, utf8*&)) {
      return String::compare<true, false>(_str, string, _size, String::size_of(string));
    } else {
      static_assert(false, "Cannot compare ShortString with provided type. Note for enforced "
                           "utf-8 encoding: Use utf8 instead of char, "
                           "and prepend string literals with 'u8': u8\"Hello world!\"");
    }
  }

  //---
  template <typename T>
  inline const ShortStringUnsafe operator +(const T& arg)
  {
    ShortStringUnsafe new_string = *this;
    ShortStringUnsafe::_append(&new_string, arg);
    return new_string;
  }

  //---
  template <typename T>
  inline ShortStringUnsafe& operator +=(const T& arg)
  {
    _append(*this, arg);
    return *this;
  }

  //---
  template <typename T>
  static inline void _append(ShortStringUnsafe& string_out, 
                             const T& arg)
  {
    if constexpr (IS_UNSAFE_LONG_STRING(T) || IS_UNSAFE_SHORT_STRING(T)) {
      u64 copy_size = Math::min((CAPACITY - 1) - string_out._size, arg._size);
      memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg._str, copy_size + 1);
      string_out._size += copy_size;
    } else if constexpr (IS_SAFE_LONG_STRING(T) || IS_SAFE_SHORT_STRING(T)) {
      u64 copy_size = Math::min((CAPACITY - 1) - string_out._size, arg.get_size());
      memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg.get_str(), copy_size + 1);
      string_out._size += copy_size;
    } else {
      String::_Internal::from_type_clip(arg, string_out._str, &string_out._size, string_out.get_capacity());
    }
  }

  //---
  template <typename... Args>
  inline void append(Args&&... args)
  {
    (_append(*this, args), ...);
  }

  //---
  inline void clear()
  {
    _str[0] = u8'\0';
    _size = 0;
  }

  //---
  constexpr u64 get_capacity() const
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
    return Math::hash(_str, _size);
  }

  //---
  template <typename T>
  ShortStringUnsafe& from_value_hex(T value)
  {
    utf8 chars[] = u8"0123456789ABCDEF";
    constexpr u32 digit_count = sizeof(T) * 2;
    constexpr u32 new_size = digit_count + 2;
    static_assert(new_size < CAPACITY, "ShortString does not have the capacity to hold result of 'from_value_hex()'.");
    _str[0] = u8'0';
    _str[1] = u8'x';
    #pragma unroll
    for (i32 d = digit_count - 1; d >= 0; --d) {
      _str[d + 2] = chars[value & 0xF];
      value >>= 4;
    }
    _str[new_size] = u8'\0';
    _size = new_size;
    return *this;
  }
};
}

//---
template <u64 CAPACITY> struct _is_unsafe_short_string<Pathlib::ShortStringUnsafe<CAPACITY>> : true_type {};