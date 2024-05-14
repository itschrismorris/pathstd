/*
  Documentation: https://www.path.blog/docs/format_type.html
*/

#pragma once
#include "memory/memcpy.h"
#include "memory/memset.h"
#include "string/strlen.h"
#include "string/from_float.h"
#include "string/from_int.h"
#include "types.h"

#define __DECORATE_INT(ARG, DECORE, SIZE) \
  { \
    memcpy(string_out->str, &decoration[DECORE], SIZE); \
    string_out->size = SIZE; \
    String::from_int(ARG, buffer, &length); \
    memcpy(&string_out->str[string_out->size], buffer, length); \
    string_out->size += length; \
  }

#define __END(DECORE, SIZE) \
  { \
    memcpy(string_out->str, &decoration[DECORE], SIZE); \
    string_out->size = SIZE; \
  }

namespace Pathlib::String {

template <typename T>
static inline u64 format_type(utf8* string_out,
                              u64 max_size,
                              const T arg)
{
  static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with 'u8' for utf-8 encoding: 'u8\"Hello world!\"'");
  static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
  if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
    u64 copy_size = Math::min(max_size - 1, strlen(arg));
    Memory::memcpy<true, false>(string_out, arg, copy_size);
    return copy_size;
  } else if constexpr (SAME_TYPE(T, i8) || SAME_TYPE(T, i16) || SAME_TYPE(T, i32) || SAME_TYPE(T, i64) ||
                       SAME_TYPE(T, u8) || SAME_TYPE(T, u16) || SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
    utf8 buffer[32];
    u64 arg_size;
    utf8* buffer_str = String::from_int(arg, buffer, &arg_size);
    u64 copy_size = Math::min(max_size - 1, arg_size);
    Memory::memcpy<true, false>(string_out, buffer_str, copy_size);
    return copy_size;
  } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
    utf8 buffer[32];
    u64 arg_size;
    utf8* buffer_str = String::from_float(arg, buffer, &arg_size);
    u64 copy_size = Math::min(max_size - 1, arg_size);
    Memory::memcpy(string_out, buffer_str, copy_size);
    return copy_size;
  } else if constexpr (SAME_TYPE(T, i32_2) || SAME_TYPE(T, u32_2))  {

  } else if constexpr (SAME_TYPE(T, i32_3) || SAME_TYPE(T, u32_3))  {

  } else if constexpr (SAME_TYPE(T, i32_4) || SAME_TYPE(T, u32_4))  {

  } else if constexpr (SAME_TYPE(T, i32_8) || SAME_TYPE(T, u32_8))  {
      

  } else if constexpr (SAME_TYPE(T, f32_2))  {

  } else if constexpr (SAME_TYPE(T, f32_3))  {

  } else if constexpr (SAME_TYPE(T, f32_4))  {

  } else if constexpr (SAME_TYPE(T, f32_8))  {

  } else {
    static_assert(false, "Unsupported type used for formatting a string.");
  }
}
}