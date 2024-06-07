/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/string/from_type.h"
#include "pathlib/string/short_string_unsafe.h"

namespace Pathlib {

//---
template <u64 RESERVE_CAPACITY>
struct LongStringUnsafe
{
  //---
  utf8* _str;
  u64 _capacity;
  u64 _size;
  utf8 _name[96];

  //---
  constexpr const utf8* type_name() const
  {
    return u8"LongString";
  }
  
  //---
  LongStringUnsafe(const utf8* name)
  {
    _capacity = RESERVE_CAPACITY;
    _str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1, 
                                name ? ShortStringUnsafe<96>(u8"[LongString]\"", name, u8"\"::[utf8*]_str")._str : nullptr);
    u64 name_length = Math::min(95LLU, String::size_of(name));
    memcpy_unsafe(_name, name, name_length);
    _name[name_length] = u8'\0';
    clear();
  }

  //---
  ~LongStringUnsafe()
  {
    if (_str) {
      free_unsafe((void**)&_str);
    }
  }

  //---
  LongStringUnsafe(const LongStringUnsafe& string)
  {
    _str = (utf8*)malloc_unsafe(string._capacity + 1, 
                                ShortStringUnsafe<96>(u8"[LongString]\"", string._name, u8"(copy)::[utf8*]_str")._str);
    _capacity = string._capacity;
    memcpy_unsafe<true, true>(_str, string._str, string._size + 1);
    _size = string._size;
  }
  
  //---
  template <typename... Args>
  LongStringUnsafe(const utf8* name,
                   Args&&... args)
  {
    _str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1, 
                               ShortStringUnsafe<96>(u8"[LongString]\"", name, u8"\"::[utf8*]_str")._str);
    _capacity = RESERVE_CAPACITY;
    _size = 0;
    (LongStringUnsafe::_append(*this, args), ...);
  }

  //---
  inline LongStringUnsafe& operator =(const LongStringUnsafe& string)
  {
    _size = string._size;
    if (_size > string._capacity) {
      _capacity = _size * 1.5;
      _str = (utf8*)realloc_unsafe(_str, _capacity + 1);
    }
    memcpy_unsafe<true, true>(_str, string._str, string._size + 1);
    _size = string._size;
    return *this;
  }

  //---
  template <typename T>
  inline LongStringUnsafe& operator =(const T arg)
  {
    u64 arg_size = String::size_of(arg);
    if (arg_size > _capacity) {
      _capacity = arg_size * 1.5;
      _str = (utf8*)realloc_unsafe(_str, _capacity + 1);
    }
    String::_Internal::from_type_grow(arg, &_str, &_size, &_capacity);
    return *this;
  }

  //---
  inline bool operator ==(const utf8* string) const
  {
    return String::compare<true, false>(_str, string, _size);
  }

  //---
  inline bool operator ==(const LongStringUnsafe& string) const
  {
    return String::compare<true, true>(_str, string._str, _size, string._size);
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
  inline LongStringUnsafe& operator +=(const LongStringUnsafe& arg)
  {
    u64 new_size = _size + arg._size;
    if (new_size > _capacity) {
      _capacity = _size * 1.5;
      _str = (utf8*)realloc_unsafe(_str, _capacity + 1);
    }
    memcpy_unsafe<false, true>(&_str[_size], arg._str, arg._size + 1);
    _size = new_size;
    return *this;
  }

  //---
  template <u64 CAPACITY>
  inline LongStringUnsafe& operator +=(const ShortStringUnsafe<CAPACITY>& arg)
  {
    u64 new_size = _size + arg._size;
    if (new_size > _capacity) {
      _capacity = _size * 1.5;
      _str = (utf8*)realloc_unsafe(_str, _capacity + 1);
    }
    memcpy_unsafe<false, true>(&_str[_size], arg._str, arg._size + 1);
    _size = new_size;
    return *this;
  }

  //---
  template <typename T>
  inline LongStringUnsafe& operator +=(const T arg)
  {
    String::_Internal::from_type_grow(arg, &_str, &_size, &_capacity);
    return *this;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongStringUnsafe& string_out, 
                             const ShortStringUnsafe<ARG_CAPACITY>& arg)
  {
    u64 new_size = string_out._size + arg._size;
    if (new_size > string_out._capacity) {
      string_out._capacity = new_size * 1.5;
      string_out._str = (utf8*)realloc_unsafe(string_out._str, string_out._capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg._str, arg._size + 1);
    string_out._size = new_size;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongStringUnsafe& string_out,
                             const LongStringUnsafe<ARG_CAPACITY>& arg)
  {
    u64 new_size = string_out._size + arg._size;
    if (new_size > string_out._capacity) {
      string_out._capacity = new_size * 1.5;
      string_out._str = (utf8*)realloc_unsafe(string_out._str, string_out._capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg._str, arg._size + 1);
    string_out._size = new_size;
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
    String::_Internal::from_type_grow(arg, &string_out._str, &string_out._size, &string_out._capacity);
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
  inline LongStringUnsafe& format(Args&&... args)
  {
    _size = 0;
    (LongStringUnsafe::_append(*this, args), ...);
    return *this;
  }

  //---
  template <typename... Args>
  static inline LongStringUnsafe format(LongStringUnsafe& string_out,
                                        Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  inline void clear()
  {
    _str[0] = u8'\0';
    _size = 0;
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
  LongStringUnsafe& from_value_hex(T value)
  {
    utf8 chars[] = u8"0123456789ABCDEF";
    constexpr u32 digit_count = sizeof(T) * 2;
    constexpr u32 new_size = digit_count + 2;
    if (new_size > _capacity) {
      _capacity = new_size * 1.5;
      _str = (utf8*)realloc_unsafe(_str, _capacity + 1);
    }
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
template <u64 RESERVE_CAPACITY> struct _is_long_string<Pathlib::LongStringUnsafe<RESERVE_CAPACITY>> : true_type {};