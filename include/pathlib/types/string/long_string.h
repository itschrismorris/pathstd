/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/types/string/from_type.h"
#include "pathlib/types/string/short_string.h"

namespace Pathlib {

//---
template <u64 RESERVE_CAPACITY>
struct LongString
{
private:
  //---
  utf8* str;
  u64 capacity;
  u64 size;
  
public:
  //---
  LongString()
  {
    capacity = RESERVE_CAPACITY;
    str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1);
    clear();
  }

  //---
  ~LongString() 
  {
    if (str) {
      free_unsafe((void**)&str);
    }
  }

  //---
  LongString(const LongString& string)
  {
    str = (utf8*)malloc_unsafe(string.capacity + 1);
    capacity = string.capacity;
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
  }
  
  //---
  template <typename... Args>
  LongString(Args&&... args)
  {
    str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1);
    capacity = RESERVE_CAPACITY;
    size = 0;
    (LongString::_append(*this, args), ...);
  }

  //---
  inline LongString& operator =(const LongString& string)
  {
    size = string.size;
    if (size > string.capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  //---
  template <typename T>
  inline LongString& operator =(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        error.set_last_error(u8"Attempt to set LongString to a null pointer.");
        error.to_log();
        error.kill_script();
        return false;
      }
    }
    u64 arg_size = String::size_of(arg);
    if (arg_size > capacity) {
      capacity = arg_size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    String::_Internal::from_type_grow(arg, &str, &size, &capacity);
    return *this;
  }

  //---
  inline bool operator ==(const utf8* string) const
  {
    if (EXPECT(string != nullptr)) {
      return String::compare<true, false>(str, string, size);
    } else {
      error.set_last_error(u8"Attempt to compare LongString equality with a null pointer.");
      error.to_log();
      error.kill_script();
      return false;
    }
  }

  //---
  inline bool operator ==(const LongString& string) const
  {
    return String::compare<true, true>(str, string.str, size, string.size);
  }

  //---
  template <typename T>
  inline const LongString operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "LongString literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    LongString::_append(*this, arg);
    return *this;
  }

  //---
  inline LongString& operator +=(const LongString& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <u64 CAPACITY>
  inline LongString& operator +=(const ShortString<CAPACITY>& arg)
  {
    u64 new_size = size + arg.size;
    if (new_size > capacity) {
      capacity = size * 1.5;
      str = (utf8*)realloc_unsafe(str, capacity + 1);
    }
    memcpy_unsafe<false, true>(&str[size], arg.str, arg.size + 1);
    size = new_size;
    return *this;
  }

  //---
  template <typename T>
  inline LongString& operator +=(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        error.set_last_error(u8"Attempt to append LongString with a null pointer.");
        error.to_log();
        error.kill_script();
        return *this;
      }
    }
    String::_Internal::from_type_grow(arg, &str, &size, &capacity);
    return *this;
  }

  //---
  template <typename... Args>
  inline void format(Args&&... args)
  {
    size = 0;
    (LongString::_append(*this, args), ...);
  }

  //---
  inline void clear()
  {
    str[0] = u8'\0';
    size = 0;
  }

  //---
  template <u64 CAPACITY>
  static inline void _append(LongString& string_out, 
                             const ShortString<CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)realloc_unsafe(string_out.str, string_out.capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <u64 CAPACITY>
  static inline void _append(LongString& string_out,
                             const LongString<CAPACITY>& arg)
  {
    u64 new_size = string_out.size + arg.size;
    if (new_size > string_out.capacity) {
      string_out.capacity = new_size * 1.5;
      string_out.str = (utf8*)realloc_unsafe(string_out.str, string_out.capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out.str[string_out.size], arg.str, arg.size + 1);
    string_out.size = new_size;
  }

  //---
  template <typename T>
  static inline void _append(LongString& string_out, 
                             const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        error.set_last_error(u8"Attempt to append LongString with a null pointer.");
        error.to_log();
        error.kill_script();
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
  static inline void append(LongString& string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  static inline LongString format(LongString& string_out,
                                  Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  static inline LongString format_copy(Args&&... args)
  {
    LongString string;
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

  //---
  template<u64 _CAPACITY>
  friend struct LongStringUnsafe;
};
}