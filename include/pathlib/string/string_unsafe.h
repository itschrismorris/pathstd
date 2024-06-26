/*
  Documentation: https://www.path.blog/docs/fixed_string_unsafe.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/string/from_type.h"
#include "pathlib/string/fixed_string_unsafe.h"

namespace Pathlib {

//---
template <u64 RESERVE_CAPACITY>
struct StringUnsafe
{
  //---
  utf8* _str;
  u64 _capacity;
  u64 _size;

  //---
  template <typename... Args>
  explicit StringUnsafe(const MemoryName& name,
                        Args&&... args)
  {
    _capacity = RESERVE_CAPACITY;
    _str = (utf8*)malloc_unsafe(RESERVE_CAPACITY + 1, FixedStringUnsafe<64>(u8"\"", name(), u8"\"::_str")._str);
    clear();
    (StringUnsafe::_append(*this, args), ...);
  }

  //---
  DISALLOW_COPY_CONSTRUCTOR(StringUnsafe);

  //---
  ~StringUnsafe()
  {
    if (_str) {
      free_unsafe((void**)&_str);
    }
  }

  //---
  template <typename T>
  inline StringUnsafe& operator =(const T& arg)
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
      return strcmp<true, true>(_str, string._str, _size, string._size);
    } else if constexpr (IS_SAFE_STRING(T) || IS_SAFE_FIXED_STRING(T)) {
      return strcmp<true, true>(_str, string.get_str(), _size, string.get_size());
    } else if constexpr (SAME_TYPE(ARRAY_TYPE(T), const utf8) || SAME_TYPE(ARRAY_TYPE(T), utf8) || 
                         SAME_TYPE(T&, const utf8*&) || SAME_TYPE(T&, utf8*&)) {
      return strcmp<true, false>(_str, string, _size, strlen(string));
    } else {
      static_assert(false, "Cannot compare LongString with provided type. Note for enforced "
                           "utf-8 encoding: Use utf8 instead of char, "
                           "and prepend string literals with 'u8': u8\"Hello world!\"");
    }
  }

  //---
  template <typename T>
  inline const StringUnsafe operator +(const T& arg)
  {
    StringUnsafe::_append(*this, arg);
    return *this;
  }

  //---
  template <typename T>
  inline StringUnsafe& operator +=(const T& arg)
  {
    _append(*this, arg);
    return *this;
  }

  //---
  template <typename T>
  static inline void _append(StringUnsafe& string_out, 
                             const T& arg)
  {
    static_assert(!SAME_TYPE(T, const char*) && !SAME_TYPE(T, char*), 
                  "UTF-8 encoding is enforced, please prepend string literals with 'u8': u8\"Hello world!\"");
    if constexpr (IS_UNSAFE_STRING(T) || IS_UNSAFE_FIXED_STRING(T)) {
      u64 new_size = string_out._size + arg._size;
      if (new_size > string_out._capacity) {
        string_out._capacity = new_size * 1.5;
        string_out._str = (utf8*)realloc_unsafe(string_out._str, string_out._capacity + 1);
      }
      memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg._str, arg._size + 1);
      string_out._size = new_size;
    } else if constexpr (IS_SAFE_STRING(T) || IS_SAFE_FIXED_STRING(T)) {
      u64 new_size = string_out._size + arg.get_size();
      if (new_size > string_out._capacity) {
        string_out._capacity = new_size * 1.5;
        string_out._str = (utf8*)realloc_unsafe(string_out._str, string_out._capacity + 1);
      }
      memcpy_unsafe<false, true>(&string_out._str[string_out._size], arg.get_str(), arg.get_size() + 1);
      string_out._size = new_size;
    } else {
      _Internal::from_type_grow(arg, &string_out._str, &string_out._size, &string_out._capacity);
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
  inline u64 get_capacity() const
  {
    return _capacity;
  }

  //---
  static inline u32 hash(const utf8* value)
  {
    return Math::hash(value, strlen(value));
  }

  //---
  inline u32 hash() const
  {
    return Math::hash(_str, _size);
  }

  //---
  template <typename T>
  StringUnsafe& from_value_hex(T value)
  {
    utf8 chars[] = u8"0123456789ABCDEF";
    constexpr u32 digit_count = sizeof(T) * 2;
    constexpr u32 new_size = digit_count + 2;
    if (new_size >= _capacity) {
      _capacity = new_size * 1.5;
      _str = (utf8*)realloc_unsafe(_str, _capacity);
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
template <u64 RESERVE_CAPACITY> struct _is_unsafe_string<Pathlib::StringUnsafe<RESERVE_CAPACITY>> : true_type {};