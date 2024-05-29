/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/types/string/from_type.h"
#include "pathlib/types/string/long_string.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/types/string/short_string_unsafe.h"

namespace Pathlib::String {

//---
template <u64 RESERVE_CAPACITY>
struct LongStringUnsafe
{
  //---
  utf8* str;
  u64 capacity;
  u64 size;
  
  //---
  LongStringUnsafe()
  {
    capacity = RESERVE_CAPACITY;
    str = (utf8*)MALLOC(RESERVE_CAPACITY + 1);
    clear();
  }

  //---
  ~LongStringUnsafe()
  {
    if (str) {
      FREE(str);
    }
  }

  //---
  LongStringUnsafe(const LongStringUnsafe& string)
  {
    str = (utf8*)MALLOC(string.capacity + 1);
    capacity = string.capacity;
    Memory::memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
  }
  
  //---
  template <typename... Args>
  LongStringUnsafe(Args&&... args)
  {
    str = (utf8*)MALLOC(RESERVE_CAPACITY + 1);
    capacity = RESERVE_CAPACITY;
    size = 0;
    (LongStringUnsafe::_append(*this, args), ...);
  }

  //---
  inline LongStringUnsafe& operator =(const LongStringUnsafe& string)
  {
    size = string.size;
    if (size > string.capacity) {
      capacity = size * 1.5;
      str = (utf8*)REALLOC(str, capacity + 1);
    }
    Memory::memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  //---
  template <typename T>
  inline LongStringUnsafe& operator =(const T arg)
  {
    u64 arg_size = String::size_of(arg);
    if (arg_size > capacity) {
      capacity = arg_size * 1.5;
      str = (utf8*)REALLOC(str, capacity + 1);
    }
    String::_Internal::from_type_grow(arg, &str, &size, &capacity);
    return *this;
  }

  //---
  inline bool operator ==(const utf8* string) const
  {
    return String::compare<true, false>(str, string, size);
  }

  //---
  inline bool operator ==(const LongStringUnsafe& string) const
  {
    return String::compare<true, true>(str, string.str, size, string.size);
  }

  //---
  template <typename T>
  inline const LongStringUnsafe operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "LongStringUnsafe literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    LongStringUnsafe::_append(*this, arg);
    return *this;
  }

  //---
  inline LongStringUnsafe& operator +=(const LongString<RESERVE_CAPACITY>& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)REALLOC(str, capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  inline LongStringUnsafe& operator +=(const LongStringUnsafe& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)REALLOC(str, capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <u64 CAPACITY>
  inline LongStringUnsafe& operator +=(const ShortString<CAPACITY>& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)REALLOC(str, capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <u64 CAPACITY>
  inline LongStringUnsafe& operator +=(const ShortStringUnsafe<CAPACITY>& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)REALLOC(str, capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <typename T>
  inline LongStringUnsafe& operator +=(const T arg)
  {
    String::_Internal::from_type_grow(arg, &str, &size, &capacity);
    return *this;
  }

  //---
  template <typename... Args>
  inline void format(Args&&... args)
  {
    size = 0;
    (LongStringUnsafe::_append(this, args), ...);
  }

  //---
  inline void clear()
  {
    str[0] = u8'\0';
    size = 0;
  }

  //---
  template <u64 CAPACITY>
  static inline void _append(LongStringUnsafe& string_out, 
                             const ShortString<CAPACITY>& arg)
  {

    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)REALLOC(string_out.str, string_out.capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <u64 CAPACITY>
  static inline void _append(LongStringUnsafe& string_out, 
                             const ShortStringUnsafe<CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)REALLOC(string_out.str, string_out.capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <u64 CAPACITY>
  static inline void _append(LongStringUnsafe& string_out,
                             const LongString<CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)REALLOC(string_out.str, string_out.capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <u64 CAPACITY>
  static inline void _append(LongStringUnsafe& string_out,
                             const LongStringUnsafe<CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)REALLOC(string_out.str, string_out.capacity + 1);
    }
    Memory::memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <typename T>
  static inline void _append(LongStringUnsafe& string_out, 
                             const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        return;
      }
    }
    String::_Internal::from_type_grow(arg, &string_out.str, &string_out.size, &string_out.capacity);
  }

  //---
  template <typename... Args>
  inline void append(Args&&... args)
  {
    (_append(*this, args), ...);
  }

  //---
  template <typename... Args>
  static inline void append(LongStringUnsafe& string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  static inline LongStringUnsafe format(LongStringUnsafe& string_out,
                                        Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  static inline LongStringUnsafe format_copy(Args&&... args)
  {
    LongStringUnsafe string;
    (_append(&string, args), ...);
    return string;
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
};
}

//---
template <typename T> struct _is_long_string : false_type {};
template <u64 RESERVE_CAPACITY> struct _is_long_string<Pathlib::String::LongString<RESERVE_CAPACITY>> : true_type {};
template <u64 RESERVE_CAPACITY> struct _is_long_string<Pathlib::String::LongStringUnsafe<RESERVE_CAPACITY>> : true_type {};