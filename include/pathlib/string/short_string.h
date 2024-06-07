/*
  Documentation: https://www.path.blog/docs/short_string_unsafe.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/string/from_type.h"
#include "pathlib/string/compare.h"
#include "pathlib/errors/errors.h"
#include "pathlib/containers/safe_ptr.h"

namespace Pathlib {

//---
template <u64 CAPACITY>
struct ShortString
{
private:
  //---
  alignas(32) utf8 _str[CAPACITY];
  u64 _size;

public:
  //---
  template <typename... Args>
  ShortString(Args&&... args)
  {
    clear();
    (_append(*this, args), ...);
  }

  //---
  ShortString(const ShortString& string)
  {
    memcpy_unsafe<true, true>(_str, string._str, string._size + 1);
    _size = string._size;
  }

  //---
  ~ShortString()
  {
  }

  //---
  operator utf8*() const
  {
    return _str;
  }

  //---
  ShortString& operator =(const ShortString& string)
  {
    memcpy_unsafe<true, true>(_str, string._str, string._size + 1);
    _size = string._size;
    return *this;
  }

  //---
  template <typename T>
  ShortString& operator =(const T& arg)
  {
    _size = 0;
    _append(*this, arg);
    return *this;
  }

  //---
  bool operator ==(const utf8* string) const
  {
    if (EXPECT(string != nullptr)) {
      return String::compare<true, false>(_str, string, _size);
    } else {
      get_errors().to_log_with_stacktrace(u8"Attempt to compare ShortString equality with a null pointer.");
      get_errors().kill_script();
      return false;
    }
  }

  //---
  bool operator ==(const ShortString& string) const
  {
    if (_size != string._size) {
      return false;
    }
    return String::compare<true, true>(_str, string._str);
  }

  //---
  template <typename T>
  const ShortString operator +(const T& arg)
  {
    static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with u8 for utf-8 encoding: u8\"Hello world!\"");
    static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
    ShortString::_append(*this, arg);
    return *this;
  }

  //---
  ShortString& operator +=(const ShortString& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - _size, arg._size);
    memcpy_unsafe<false, true>(&_str[_size], arg._str, copy_size);
    _size += copy_size;
    return *this;
  }

  //---
  template <typename T>
  ShortString& operator +=(const T& arg)
  {
    _append(*this, arg);
    return *this;
  }

  //---
  template <u64 ARG_CAPACITY>
  static void _append(ShortString& string_out, 
                      const ShortString<ARG_CAPACITY>& arg)
  {
    u64 copy_size = Math::min((CAPACITY - 1) - string_out._size, arg.get_size());
    memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg.get_str(), copy_size + 1);
    string_out._size += copy_size;
  }

  //---
  template <typename T>
  static void _append(ShortString& string_out, 
                      const T& arg)
  {
    if constexpr (IS_POINTER(T)) {
      if (DONT_EXPECT(arg == nullptr)) {
        get_errors().to_log(u8"Attempt to _append() a nullptr to ShortString.");
        get_errors().kill_script();
        return;
      }
    }
    if constexpr (IS_UNSAFE_LONG_STRING(T) || IS_UNSAFE_SHORT_STRING(T)) {
      String::_Internal::from_type_grow(arg._str, &string_out._str, &string_out._size, &string_out._capacity);
    } else if constexpr (IS_LONG_STRING(T) || IS_SHORT_STRING(T)) {
      String::_Internal::from_type_grow(arg.get_str().get_ptr(), &string_out._str, &string_out._size, &string_out._capacity);
    }else {
      String::_Internal::from_type_grow(arg, &string_out._str, &string_out._size, &string_out._capacity);
    }
  }

  //---
  template <typename... Args>
  void append(Args&&... args)
  {
    (_append(*this, args), ...);
  }

  //---
  template <typename... Args>
  static void append(ShortString& string_out,
                     Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  template <typename... Args>
  ShortString& format(Args&&... args)
  {
    _size = 0;
    (ShortString::_append(*this, args), ...);
    return *this;
  }

  //---
  template <typename... Args>
  static ShortString format(ShortString& string_out,
                            Args&&... args)
  {
    (_append(string_out, args), ...);
  }

  //---
  void clear()
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
  u32 hash() const 
  {
    return Math::hash(_str, _size);
  }

  //---
  template <typename T>
  ShortString& from_value_hex(T value)
  {
    utf8 chars[] = u8"0123456789ABCDEF";
    constexpr u32 digit_count = sizeof(T) * 2;
    constexpr u32 new_size = digit_count + 2;
    static_assert(new_size < CAPACITY, "ShortString does not have the capacity to hold result of 'from_value_hex()'.");
    _str[0] = u8'0';
    _str[1] = u8'x';
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
template <u64 CAPACITY> struct _is_short_string<Pathlib::ShortString<CAPACITY>> : true_type {};