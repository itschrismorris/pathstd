/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/string/from_type.h"
#include "pathlib/string/short_string.h"
#include "pathlib/profiler/profiler.h"

namespace Pathlib {

//---
template <u64 RESERVE_CAPACITY>
struct LongString
{
private:
  //---
  utf8* _str;
  u64 _capacity;
  u64 _size;
  
public:
  //---
  LongString(const utf8* name)
  {
    _capacity = RESERVE_CAPACITY;
    _str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1, ShortStringUnsafe<96>(name, u8"::str").str);
    clear();
  }

  //---
  ~LongString() 
  {
    if (_str) {
      free_unsafe((void**)&_str);
    }
  }

  //---
  LongString(const LongString& string)
  {
    _str = (utf8*)malloc_unsafe(string._capacity + 1, 
                               ShortStringUnsafe<96>(_Internal::Profiler::get_memory_item_name(string._str), u8"(copy)::str").str);
    _capacity = string._capacity;
    memcpy_unsafe<true, true>(_str, string._str, string._size + 1);
    _size = string._size;
  }
  
  //---
  template <typename... Args>
  LongString(const utf8* name,
             Args&&... args)
  {
    _str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1, ShortStringUnsafe<96>(name, u8"::str").str);
    _capacity = RESERVE_CAPACITY;
    _size = 0;
    (LongString::_append(*this, args), ...);
  }

  //---
  inline LongString& operator =(const LongString& string)
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
  inline LongString& operator =(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        get_errors().set_last_error(u8"Attempt to set LongString to a null pointer.");
        get_errors().to_log();
        get_errors().kill_script();
        return false;
      }
    }
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
    if (EXPECT(string != nullptr)) {
      return String::compare<true, false>(_str, string, _size);
    } else {
      get_errors().set_last_error(u8"Attempt to compare LongString equality with a null pointer.");
      get_errors().to_log();
      get_errors().kill_script();
      return false;
    }
  }

  //---
  inline bool operator ==(const LongString& string) const
  {
    return String::compare<true, true>(_str, string._str, _size, string._size);
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
  inline LongString& operator +=(const ShortString<CAPACITY>& arg)
  {
    u64 new_size = _size + arg.size;
    if (new_size > _capacity) {
      _capacity = _size * 1.5;
      _str = (utf8*)realloc_unsafe(_str, _capacity + 1);
    }
    memcpy_unsafe<false, true>(&_str[_size], arg.str, arg.size + 1);
    _size = new_size;
    return *this;
  }

  //---
  template <typename T>
  inline LongString& operator +=(const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        get_errors().set_last_error(u8"Attempt to append LongString with a null pointer.");
        get_errors().to_log();
        get_errors().kill_script();
        return *this;
      }
    }
    String::_Internal::from_type_grow(arg, &_str, &_size, &_capacity);
    return *this;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongString& string_out, 
                             const ShortString<ARG_CAPACITY>& arg)
  {
    u64 new_size = string_out._size + arg.get_size();
    if (new_size > string_out._capacity) {
      string_out._capacity = new_size * 1.5;
      string_out._str = (utf8*)realloc_unsafe(string_out._str, string_out._capacity + 1);
    }
    memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg.get_str(), arg.get_size() + 1);
    string_out._size = new_size;
  }

  //---
  template <u64 ARG_CAPACITY>
  static inline void _append(LongString& string_out,
                             const LongString<ARG_CAPACITY>& arg)
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
  static inline void _append(LongString& string_out, 
                             const T arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (!arg) {
        get_errors().set_last_error(u8"Attempt to append LongString with a null pointer.");
        get_errors().to_log();
        get_errors().kill_script();
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
  static inline void append(LongString& string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  inline LongString& format(Args&&... args)
  {
    _size = 0;
    (LongString::_append(*this, args), ...);
    return *this;
  }

  //---
  template <typename... Args>
  static inline LongString format(LongString& string_out,
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
  u64 get_size() const
  {
    return _size;
  }

  //---
  const SafePtr<utf8> get_str() const
  {
    SafePtr<utf8> ptr = _str;
    ptr.set_count(_size);
    return ptr;
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
  LongString& from_value_hex(T value)
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