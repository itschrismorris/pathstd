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

#define __DECORATE(DECORATE, DECORATE_SIZE) \
  { \
    u64 copy_size = Math::min(string_capacity - 1 - total_size, DECORATE_SIZE); \
    Memory::memcpy(&string_out[total_size], &decorate[DECORATE], copy_size); \
    total_size += copy_size; \
  }

#define __DECORATE_INT(ARG, DECORATE, DECORATE_SIZE) \
  { \
    u64 copy_size = Math::min(string_capacity - 1 - total_size, DECORATE_SIZE); \
    Memory::memcpy(&string_out[total_size], &decorate[DECORATE], copy_size); \
    total_size += copy_size; \
    utf8* buffer_str = String::from_int(ARG, buffer, &conversion_size); \
    copy_size = Math::min(string_capacity - 1 - total_size, conversion_size); \
    Memory::memcpy(&string_out[total_size], buffer_str, copy_size); \
    total_size += copy_size; \
  }

#define __DECORATE_FLOAT(ARG, DECORATE, DECORATE_SIZE) \
  { \
    u64 copy_size = Math::min(string_capacity - 1 - total_size, DECORATE_SIZE); \
    Memory::memcpy(&string_out[total_size], &decorate[DECORATE], copy_size); \
    total_size += copy_size; \
    utf8* buffer_str = String::from_float(ARG, buffer, &conversion_size); \
    copy_size = Math::min(string_capacity - 1 - total_size, conversion_size); \
    Memory::memcpy(&string_out[total_size], buffer_str, copy_size); \
    total_size += copy_size; \
  }

namespace Pathlib::String {

template <typename T>
static inline u64 format_type(utf8* string_out,
                              u64 string_capacity,
                              const T arg)
{
  static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with 'u8' for utf-8 encoding: 'u8\"Hello world!\"'");
  static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
  if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
    u64 copy_size = Math::min(string_capacity - 1, strlen(arg));
    Memory::memcpy(string_out, arg, copy_size);
    return copy_size;
  } else if constexpr (SAME_TYPE(T, i8) || SAME_TYPE(T, i16) || SAME_TYPE(T, i32) || SAME_TYPE(T, i64) ||
                       SAME_TYPE(T, u8) || SAME_TYPE(T, u16) || SAME_TYPE(T, u32) || SAME_TYPE(T, u64)) {
    utf8 buffer[32];
    u64 size;
    utf8* buffer_str = String::from_int(arg, buffer, &size);
    u64 copy_size = Math::min(string_capacity - 1, size);
    Memory::memcpy(string_out, buffer_str, copy_size);
    return copy_size;
  } else if constexpr (SAME_TYPE(T, f32) || SAME_TYPE(T, f64)) {
    utf8 buffer[32];
    u64 size;
    utf8* buffer_str = String::from_float(arg, buffer, &size);
    u64 copy_size = Math::min(string_capacity - 1, size);
    Memory::memcpy(string_out, buffer_str, copy_size);
    return copy_size;
  } else if constexpr (SAME_TYPE(T, i32_2) || SAME_TYPE(T, u32_2))  {
    const utf8* decorate = u8"{ x: , y: }";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_INT(arg.x, 0LLU, 5LLU);
    __DECORATE_INT(arg.y, 5LLU, 5LLU);
    __DECORATE(9LLU, 2LLU);
    return total_size;
  } else if constexpr (SAME_TYPE(T, i32_3) || SAME_TYPE(T, u32_3))  {
    const utf8* decorate = u8"{ x: , y: , z: }";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_INT(arg.x, 0LLU, 5LLU);
    __DECORATE_INT(arg.y, 5LLU, 5LLU);
    __DECORATE_INT(arg.z, 10LLU, 5LLU);
    __DECORATE(14LLU, 2LLU);
    return total_size;
  } else if constexpr (SAME_TYPE(T, i32_4) || SAME_TYPE(T, u32_4))  {
    const utf8* decorate = u8"{ x: , y: , z: , w: }";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_INT(arg.x, 0LLU, 5LLU);
    __DECORATE_INT(arg.y, 5LLU, 5LLU);
    __DECORATE_INT(arg.z, 10LLU, 5LLU);
    __DECORATE_INT(arg.w, 15LLU, 5LLU);
    __DECORATE(19LLU, 2LLU);
    return total_size;
  } else if constexpr (SAME_TYPE(T, i32_8) || SAME_TYPE(T, u32_8))  {
    const utf8* decorate = u8"{ lo: { x: , y: , z: , w: }, hi: ";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_INT(arg.lo.x, 0LLU, 11LLU);
    __DECORATE_INT(arg.lo.y, 11LLU, 5LLU);
    __DECORATE_INT(arg.lo.z, 16LLU, 5LLU);
    __DECORATE_INT(arg.lo.w, 21LLU, 5LLU);
    __DECORATE(25LLU, 8LLU);
    __DECORATE_INT(arg.hi.x, 6LLU, 5LLU);
    __DECORATE_INT(arg.hi.y, 11LLU, 5LLU);
    __DECORATE_INT(arg.hi.z, 16LLU, 5LLU);
    __DECORATE_INT(arg.hi.w, 21LLU, 5LLU);
    __DECORATE(25LLU, 2LLU);
    return total_size;
  } else if constexpr (SAME_TYPE(T, f32_2))  {
    const utf8* decorate = u8"{ x: , y: }";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_FLOAT(arg.x, 0LLU, 5LLU);
    __DECORATE_FLOAT(arg.y, 5LLU, 5LLU);
    __DECORATE(9LLU, 2LLU);
    return total_size;

  } else if constexpr (SAME_TYPE(T, f32_3))  {
    const utf8* decorate = u8"{ x: , y: , z: }";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_FLOAT(arg.x, 0LLU, 5LLU);
    __DECORATE_FLOAT(arg.y, 5LLU, 5LLU);
    __DECORATE_FLOAT(arg.z, 10LLU, 5LLU);
    __DECORATE(14LLU, 2LLU);
    return total_size;

  } else if constexpr (SAME_TYPE(T, f32_4))  {
    const utf8* decorate = u8"{ x: , y: , z: , w: }";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_FLOAT(arg.x, 0LLU, 5LLU);
    __DECORATE_FLOAT(arg.y, 5LLU, 5LLU);
    __DECORATE_FLOAT(arg.z, 10LLU, 5LLU);
    __DECORATE_FLOAT(arg.w, 15LLU, 5LLU);
    __DECORATE(19LLU, 2LLU);
    return total_size;

  } else if constexpr (SAME_TYPE(T, f32_8))  {
    const utf8* decorate = u8"{ lo: { x: , y: , z: , w: }, hi: ";
    utf8 buffer[32];
    u64 conversion_size;
    u64 total_size = 0;
    __DECORATE_FLOAT(arg.lo.x, 0LLU, 11LLU);
    __DECORATE_FLOAT(arg.lo.y, 11LLU, 5LLU);
    __DECORATE_FLOAT(arg.lo.z, 16LLU, 5LLU);
    __DECORATE_FLOAT(arg.lo.w, 21LLU, 5LLU);
    __DECORATE(25LLU, 8LLU);
    __DECORATE_FLOAT(arg.hi.x, 6LLU, 5LLU);
    __DECORATE_FLOAT(arg.hi.y, 11LLU, 5LLU);
    __DECORATE_FLOAT(arg.hi.z, 16LLU, 5LLU);
    __DECORATE_FLOAT(arg.hi.w, 21LLU, 5LLU);
    __DECORATE(25LLU, 2LLU);
    __DECORATE(25LLU, 2LLU);
    return total_size;
  } else {
    static_assert(false, "Unsupported type used for formatting a string.");
  }
}
}