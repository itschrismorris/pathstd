/*
  Documentation: https://www.path.blog/docs/fixed_string_unsafe.html
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
struct FixedString
{
private:
  //---
  alignas(32) utf8 _str[CAPACITY];
  u64 _size;

public:
  //---
  template <typename... Args>
  FixedString(Args&&... args)
  {
    clear();
    (_append(*this, args), ...);
  }

  //---
  FixedString(const FixedString& string)
  {
    memcpy_unsafe<true, true>(_str, string._str, string._size + 1);
    _size = string._size;
  }

  //---
  ~FixedString() {}

  //---
  template <typename T>
  FixedString& operator =(const T& arg)
  {
    _size = 0;
    _append(*this, arg);
    return *this;
  }

  //---
  template <typename T>
  inline bool operator ==(T& string)
  {
    if constexpr (IS_UNSAFE_STRING(T) || IS_UNSAFE_FIXED_STRING(T)) {
      return StringUtilities::compare<true, true>(_str, string._str, _size, string._size);
    } else if constexpr (IS_SAFE_STRING(T) || IS_SAFE_FIXED_STRING(T)) {
      return StringUtilities::compare<true, true>(_str, string.get_str(), _size, string.get_size());
    } else if constexpr (SAME_TYPE(ARRAY_TYPE(T), const utf8) || SAME_TYPE(ARRAY_TYPE(T), utf8) || 
                         SAME_TYPE(T&, const utf8*&) || SAME_TYPE(T&, utf8*&)) {
      return StringUtilities::compare<true, false>(_str, string, _size, StringUtilities::length_of(string));
    } else {
      static_assert(false, "Cannot compare ShortString with provided type. Note for enforced "
                           "utf-8 encoding: Use utf8 instead of char, "
                           "and prepend string literals with 'u8': u8\"Hello world!\"");
    }
  }

  //---
  template <typename T>
  const FixedString operator +(const T& arg)
  {
    FixedString::_append(*this, arg);
    return *this;
  }

  //---
  template <typename T>
  FixedString& operator +=(const T& arg)
  {
    _append(*this, arg);
    return *this;
  }

  //---
  template <typename T>
  static void _append(FixedString& string_out, 
                      const T& arg)
  {
    static_assert(!SAME_TYPE(T, const char*) && !SAME_TYPE(T, char*), 
                  "UTF-8 encoding is enforced, please prepend string literals with 'u8': u8\"Hello world!\"");
    if constexpr (IS_POINTER(T)) {
      if (DONT_EXPECT(arg == nullptr)) {
        get_errors().to_log(u8"Attempt to _append() a nullptr to ShortString.");
        get_errors().kill_script();
        return;
      }
    }
    if constexpr (IS_UNSAFE_STRING(T) || IS_UNSAFE_FIXED_STRING(T)) {
      u64 copy_size = Math::min((CAPACITY - 1) - string_out._size, arg._size);
      memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg._str, copy_size + 1);
      string_out._size += copy_size;
    } else if constexpr (IS_SAFE_STRING(T) || IS_SAFE_FIXED_STRING(T)) {
      u64 copy_size = Math::min((CAPACITY - 1) - string_out._size, arg.get_size());
      memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg.get_str(), copy_size + 1);
      string_out._size += copy_size;
    } else {
      StringUtilities::_Internal::from_type_clip(arg, string_out._str, &string_out._size, string_out.get_capacity());
    }
  }

  //---
  template <typename... Args>
  void append(Args&&... args)
  {
    (_append(*this, args), ...);
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
  FixedString& from_value_hex(T value)
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
template <u64 CAPACITY> struct _is_safe_fixed_string<Pathlib::FixedString<CAPACITY>> : true_type {};