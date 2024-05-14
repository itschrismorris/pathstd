/*
  Documentation: https://www.path.blog/docs/local_wstring.html
*/

#pragma once
#include "memory/memcpy.h"
#include "memory/memset.h"
#include "string/strlen.h"
#include "string/from_float.h"
#include "string/from_int.h"
#include "types.h"

namespace Pathlib::String {

/**/
template <u64 CAPACITY>
struct LocalString
{
  static_assert(Math::is_multiple_of<32>(CAPACITY), "LocalString CAPACITY must be a multiple of 32 (AVX purposes).");

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
    Memory::memcpy<true, true>(str, string.str, string.size);
    size = string.size;
    return *this;
  }

  /**/
  template <typename T>
  inline LocalString& operator =(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
      u64 copy_size = Math::min(CAPACITY - 1, strlen(arg));
      Memory::memcpy<true, false>(str, arg, copy_size);
      size = copy_size;
      Memory::memset(&str[size], 0x00, CAPACITY - size);
      return *this;
    } else if constexpr (SAME_TYPE(T, i8) || SAME_TYPE(T, i16) || SAME_TYPE(T, i32) || SAME_TYPE(T, i64) ||
                         SAME_TYPE(T, u8) || SAME_TYPE(T, u16) || SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
      utf8 buffer[32];
      u32 arg_size;
      utf8* buffer_str = String::from_int(arg, buffer, &arg_size);
      u64 copy_size = Math::min(CAPACITY - 1, (u64)arg_size);
      Memory::memcpy<true, false>(str, buffer_str, copy_size);
      size = copy_size;
      Memory::memset(&str[size], 0x00, CAPACITY - size);
      return *this;
    } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
      utf8 buffer[32];
      u32 arg_size;
      utf8* buffer_str = String::from_float(arg, buffer, &arg_size);
      u64 copy_size = Math::min(CAPACITY - 1, (u64)arg_size);
      Memory::memcpy<true, false>(str, buffer_str, copy_size);
      size = copy_size;
      Memory::memset(&str[size], 0x00, CAPACITY - size);
      return *this;
    }
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
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
      u64 copy_size = Math::min((CAPACITY - 1) - size, strlen(arg));
      Memory::memcpy(&str[size], arg, copy_size);
      size += copy_size;
      return *this;
    } else if constexpr (SAME_TYPE(T, i8) || SAME_TYPE(T, i16) || SAME_TYPE(T, i32) || SAME_TYPE(T, i64) ||
                         SAME_TYPE(T, u8) || SAME_TYPE(T, u16) || SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
      utf8 buffer[32];
      u32 arg_size;
      utf8* buffer_str = String::from_int(arg, buffer, &arg_size);
      u64 copy_size = Math::min((CAPACITY - 1) - size, (u64)arg_size);
      Memory::memcpy(&str[size], buffer_str, copy_size);
      size += copy_size;
      return *this;
    } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
      utf8 buffer[32];
      u32 arg_size;
      utf8* buffer_str = String::from_float(arg, buffer, &arg_size);
      u64 copy_size = Math::min((CAPACITY - 1) - size, (u64)arg_size);
      Memory::memcpy(&str[size], buffer_str, copy_size);
      size += copy_size;
      return *this;
    }
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
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
      u64 copy_size = Math::min((CAPACITY - 1) - string_out->size, strlen(arg));
      Memory::memcpy(&string_out->str[string_out->size], arg, copy_size);
      string_out->size += copy_size;
    } else if constexpr (SAME_TYPE(T, i8) || SAME_TYPE(T, i16) || SAME_TYPE(T, i32) || SAME_TYPE(T, i64) || 
                         SAME_TYPE(T, u8) || SAME_TYPE(T, u16) || SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
      utf8 buffer[32];
      u32 arg_size;
      utf8* buffer_str = String::from_int(arg, buffer, &arg_size);
      u64 copy_size = Math::min((CAPACITY - 1) - string_out->size, (u64)arg_size);
      Memory::memcpy(&string_out->str[string_out->size], buffer_str, copy_size);
      string_out->size += copy_size;
    } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
      utf8 buffer[32];
      u32 arg_size;
      utf8* buffer_str = String::from_float(arg, buffer, &arg_size);
      u64 copy_size = Math::min((CAPACITY - 1) - string_out->size, (u64)arg_size);
      Memory::memcpy(&string_out->str[string_out->size], buffer_str, copy_size);
      string_out->size += copy_size;
    } else {
      static_assert(false, "Unsupported type used for appending to LocalString.");
    }
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