/*
  Documentation: https://www.path.blog/docs/short_string.html
*/

#pragma once
#include "types/string/from_type.h"
#include "types/string/compare.h"

namespace Pathlib::String {

/**/
template <u64 CAPACITY>
struct ShortString
{
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
    Memory::memcpy<true, true>(str, string.str, string.size + 1);
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
  }

  /**/
  inline ShortString& operator =(const ShortString& string)
  {
    Memory::memcpy<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  /**/
  template <typename T>
  inline ShortString& operator =(const T arg)
  {
    String::from_type_clip(arg, str, &size, CAPACITY);
    return *this;
  }

  /**/
  inline bool operator ==(const utf8* string) const
  {
    return String::compare<true, false>(str, string, size);
  }

  /**/
  inline bool operator ==(const ShortString& string) const
  {
    if (size != string.size) {
      return false;
    }
    return String::compare<true, true>(str, string.str);
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
    String::from_type_clip(arg, str, &size, CAPACITY);
    return *this;
  }

  /**/
  template <typename... Args>
  inline void format(Args&&... args)
  {
    size = 0;
    (ShortString::_append(this, args), ...);
  }

  /**/
  inline void clear()
  {
    str[0] = u8'\0';
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
    Memory::memcpy<false, true>(&string_out->str[string_out->size], arg.str, copy_size + 1);
    string_out->size += copy_size;
  }

  /**/
  template <typename T>
  static inline void _append(ShortString* string_out, 
                             const T arg)
  {
    String::from_type_clip(arg, string_out->str, &string_out->size, string_out->capacity());
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
    ShortString string;
    (_append(&string, args), ...);
    return string;
  }

  /**/
  static inline u32 hash(const utf8* value)
  {
    u64 size = String::size_of(value);
    u32 hash = Math::hash(((u32*)value)[0]);
    for (u32 b = 1; b < (size + 3) >> 2; ++b) {
      hash ^= Math::hash(((u32*)value)[b]);
    }
    return hash;
  }

  /**/
  inline u32 hash() const 
  {
    u32 hash = Math::hash(((u32*)str)[0]);
    for (u32 b = 1; b < (size + 3) >> 2; ++b) {
      hash ^= Math::hash(((u32*)str)[b]);
    }
    return hash;
  }
};
}

/**/
template <typename T> struct _is_short_string : false_type {};
template <u64 CAPACITY> struct _is_short_string<Pathlib::String::ShortString<CAPACITY>> : true_type {};