/*
  Documentation: https://www.path.blog/docs/short_string.html
*/

#pragma once
#include "string/from_type.h"

namespace Pathlib::String {

/**/
template <u64 RESERVE_CAPACITY = 256>
struct LongString
{
  /**/
  utf8* str;
  u64 capacity;
  u64 size;
  
  /**/
  LongString()
  {
    str = (utf8*)Memory::malloc(RESERVE_CAPACITY);
    capacity = RESERVE_CAPACITY;
    clear();
  }

  /**/
  ~LongString() 
  {
    if (str) {
      Memory::free((void**)&str);
    }
  }

  /**/
  LongString(const LongString& string)
  {
    str = (utf8*)Memory::malloc(string.capacity);
    capacity = string.capacity;
    Memory::memcpy<true, true>(str, string.str, string.size + 1);
    size = string.size;
  }
  
  /**/
  template <typename... Args>
  LongString(Args&&... args)
  {
    str = (utf8*)Memory::malloc(RESERVE_CAPACITY);
    capacity = RESERVE_CAPACITY;
    size = 0;
    (LongString::_append(this, args), ...);
  }

  /**/
  inline LongString& operator =(const LongString& string)
  {
    if (capacity < string.capacity) {
      if (str) {
        Memory::realloc(str, string.capacity);
      } else {
        str = Memory::malloc(string.capacity);
      }
    }
    capacity = string.capacity;
    Memory::memcpy<true, true>(str, string.str, string.size + 1);
    size = string.size;
    return *this;
  }

  /**/
  template <typename T>
  inline LongString& operator =(const T arg)
  {
    u64 arg_size = String::size_of(arg);
    if (arg_size > capacity - 1) {
      capacity = Math::next_pot(arg_size + 1);
      Memory::realloc(str, capacity);
    }
    size = String::from_type(arg, str, capacity);
    return *this;
  }

  /**/
  inline bool operator ==(const LongString& string) const
  {
    u32 bitmask = U32_MAX;
    #pragma unroll
    for (u32 c = 0; c < (capacity >> 5); ++c) {
      I8 ours = I8_LOAD(&str[c * 32]);
      I8 theirs = I8_LOAD(&string.str[c * 32]);
      bitmask &= I8_MOVEMASK(I8_CMP_EQ(ours, theirs));
    }
    return (bitmask == U32_MAX);
  }

  /**/
  template <typename T>
  inline const LongString operator +(const T arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "LongString literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    LongString new_string = *this;
    LongString::_append(&new_string, arg);
    return new_string;
  }

  /**/
  inline LongString& operator +=(const LongString& arg)
  {
    u64 copy_size = Math::min((capacity - 1) - size, arg.size);
    Memory::memcpy<false, true>(&str[size], arg.str, copy_size);
    size += copy_size;
    return *this;
  }

  /**/
  template <typename T>
  inline LongString& operator +=(const T arg)
  {
    size += String::from_type(arg, &str[size], capacity - size);
    return *this;
  }

  /**/
  template <typename... Args>
  inline void format(Args&&... args)
  {
    size = 0;
    (LongString::_append(this, args), ...);
    Memory::memset(&str[size], 0x00, capacity - size);
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
  static inline void _append(LongString* string_out, 
                             const LongString& arg)
  {
    if ((string_out->size + arg.size) > (string_out->capacity - 1)) {
      string_out->capacity = Math::next_pot(string_out->size + arg.size + 1);
      Memory::realloc(string_out->str, string_out->capacity);
    }
    Memory::memcpy<false, true>(&string_out->str[string_out->size], arg.str, arg.size + 1);
    string_out->size += arg.size;
  }

  /**/
  template <typename T>
  static inline void _append(LongString* string_out, 
                             const T arg)
  {
    u64 arg_size = String::size_of(arg);
    if ((string_out->size + arg_size) > (string_out->capacity - 1)) {
      string_out->capacity = Math::next_pot(string_out->size + arg_size + 1);
      Memory::realloc(string_out->str, string_out->capacity);
    }
    string_out->size += String::from_type(arg, &string_out->str[string_out->size], string_out->capacity - string_out->size);
  }

  /**/
  template <typename... Args>
  static inline void append(LongString* string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  /**/
  template <typename... Args>
  static inline LongString format(LongString* string_out,
                                   Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  /**/
  template <typename... Args>
  static inline LongString format_copy(Args&&... args)
  {
    LongString string_out;
    (_append(&string_out, args), ...);
    return string_out;
  }
};
}

/**/
template <typename T> struct _is_long_string : false_type {};
template <u64 RESERVE_CAPACITY> struct _is_long_string<Pathlib::String::LongString<RESERVE_CAPACITY>> : true_type {};