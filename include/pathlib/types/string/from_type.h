/*
  Documentation: https://www.path.blog/docs/format_type.html
*/

#pragma once
#include "pathlib/memory/memcpy.h"
#include "pathlib/memory/memset.h"
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/types/string/size_of.h"
#include "pathlib/types/types.h"

namespace Pathlib::String::_Internal {

//---
static inline u16 const two_digits[100] =
{
  0x3030, 0x3130, 0x3230, 0x3330, 0x3430, 0x3530, 0x3630, 0x3730, 0x3830, 0x3930,
  0x3031, 0x3131, 0x3231, 0x3331, 0x3431, 0x3531, 0x3631, 0x3731, 0x3831, 0x3931,
  0x3032, 0x3132, 0x3232, 0x3332, 0x3432, 0x3532, 0x3632, 0x3732, 0x3832, 0x3932,
  0x3033, 0x3133, 0x3233, 0x3333, 0x3433, 0x3533, 0x3633, 0x3733, 0x3833, 0x3933,
  0x3034, 0x3134, 0x3234, 0x3334, 0x3434, 0x3534, 0x3634, 0x3734, 0x3834, 0x3934,
  0x3035, 0x3135, 0x3235, 0x3335, 0x3435, 0x3535, 0x3635, 0x3735, 0x3835, 0x3935,
  0x3036, 0x3136, 0x3236, 0x3336, 0x3436, 0x3536, 0x3636, 0x3736, 0x3836, 0x3936,
  0x3037, 0x3137, 0x3237, 0x3337, 0x3437, 0x3537, 0x3637, 0x3737, 0x3837, 0x3937,
  0x3038, 0x3138, 0x3238, 0x3338, 0x3438, 0x3538, 0x3638, 0x3738, 0x3838, 0x3938,
  0x3039, 0x3139, 0x3239, 0x3339, 0x3439, 0x3539, 0x3639, 0x3739, 0x3839, 0x3939
};

//---
template <typename T>
static inline utf8* from_number(T value,
                                utf8* buffer,
                                u64* size_out = nullptr)
{
  if constexpr (IS_INTEGRAL(T)) {
    u64 v = Math::abs(value);
    utf8* output;
    if constexpr (sizeof(T) == 4) {
      output = &buffer[11];
    }
    else {
      output = &buffer[21];
    }
    *output = '\0';
    while (v >= 100) {
      u64 const old = v;
      output -= 2;
      v /= 100;
      Memory::memcpy(output, &two_digits[old - (v * 100)], sizeof(u16));
    }
    output -= 2;
    Memory::memcpy(output, &two_digits[v], sizeof(u16));
    bool is_single_digit = (v < 10);
    output += is_single_digit;
    if (value < 0) {
      output[-1] = '-';
      output -= 1;
    }
    if (size_out) {
      if constexpr (sizeof(T) == 4) {
        *size_out = &buffer[11] - output;
      }
      else {
        *size_out = &buffer[21] - output;
      }
    }
    return output;
  }
  else if constexpr (IS_FLOAT(T)) {
    f64 v = value;
    if (Math::is_inf(v)) {
      buffer[0] = 'i'; buffer[1] = 'n'; buffer[2] = 'f';
      return buffer;
    }
    else if (Math::is_nan(v)) {
      buffer[0] = 'n'; buffer[1] = 'a'; buffer[2] = 'n';
      return buffer;
    }
    utf8* output = buffer;
    i32 whole_number = (i32)v;
    u64 whole_number_size;
    output = from_number(whole_number, output, &whole_number_size);
    output[whole_number_size] = '.';
    u32 fractional_number = (u32)(Math::abs(v - (u32)whole_number) * 1000000.0);
    utf8* fractional_start = &output[whole_number_size + 1];
    i32 divisor = 100000;
    for (u32 f = 0; f < 5; ++f) {
      if (fractional_number < divisor) {
        output[whole_number_size + f + 1] = '0';
        fractional_start += 1;
      }
      divisor /= 10;
    }
    utf8* fraction_string = fractional_start;
    u64 fraction_size;
    fraction_string = from_number(fractional_number, fraction_string, &fraction_size);
    Memory::memcpy(fractional_start, fraction_string, sizeof(utf8) * fraction_size);
    if (size_out) {
      *size_out = whole_number_size + 1 + 6;
    }
    output[whole_number_size + 1 + 6] = '\0';
    return output;
  }
}

//---
#define __PATHLIB_DECORATE_CLIP(DECORATE, DECORATE_SIZE) \
  { \
    u64 copy_size = Math::min(string_capacity - 1 - size_added, DECORATE_SIZE); \
    Memory::memcpy(&string[*string_size + size_added], &decorate[DECORATE], copy_size); \
    size_added += copy_size; \
  }

//---
#define __PATHLIB_DECORATE_NUMBER_CLIP(ARG, DECORATE, DECORATE_SIZE) \
  { \
    u64 copy_size = Math::min(string_capacity - 1 - size_added, DECORATE_SIZE); \
    Memory::memcpy(&string[*string_size + size_added], &decorate[DECORATE], copy_size); \
    size_added += copy_size; \
    utf8* buffer_str = from_number(ARG, buffer, &conversion_size); \
    copy_size = Math::min(string_capacity - 1 - size_added, conversion_size); \
    Memory::memcpy(&string[*string_size + size_added], buffer_str, copy_size); \
    size_added += copy_size; \
  }

//---
#define __PATHLIB_DECORATE_GROW(DECORATE, DECORATE_SIZE) \
  { \
    u64 new_size = *string_size + DECORATE_SIZE; \
    if (new_size > *string_capacity) { \
      *string_capacity = new_size * 1.5; \
      *string = (utf8*)REALLOC(*string, *string_capacity + 1); \
    } \
    Memory::memcpy(&(*string)[*string_size], &decorate[DECORATE], DECORATE_SIZE); \
    *string_size = new_size; \
  }

//---
#define __PATHLIB_DECORATE_NUMBER_GROW(ARG, DECORATE, DECORATE_SIZE) \
  { \
    utf8* buffer_str = from_number(ARG, buffer, &conversion_size); \
    u64 new_size = *string_size + DECORATE_SIZE + conversion_size; \
    if (new_size > *string_capacity) { \
      *string_capacity = new_size * 1.5; \
      *string = (utf8*)REALLOC(*string, *string_capacity + 1); \
    } \
    Memory::memcpy(&(*string)[*string_size], &decorate[DECORATE], DECORATE_SIZE); \
    Memory::memcpy(&(*string)[*string_size + DECORATE_SIZE], buffer_str, conversion_size); \
    *string_size = new_size; \
  }

//---
template <typename T>
static inline void from_type_clip(const T arg,
                                  utf8* string,
                                  u64* string_size,
                                  u64 string_capacity)
{
  static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with 'u8' for utf-8 encoding: 'u8\"Hello world!\"'");
  static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
  if constexpr (SAME_TYPE(T, utf8)) {
    if ((*string_size + 1) < string_capacity) {
      string[*string_size] = arg;
      string[*string_size + 1] = u8'\0';
      *string_size += 1;
    }
  } else if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
    u64 copy_size = Math::min(string_capacity - *string_size - 1, String::size_of(arg));
    Memory::memcpy(&string[*string_size], arg, copy_size);
    *string_size += copy_size;
    string[*string_size] = u8'\0';
  } else if constexpr (IS_INTEGRAL(T) || IS_FLOAT(T)) {
    utf8 buffer[32];
    u64 conversion_size;
    utf8* buffer_str = from_number(arg, buffer, &conversion_size);
    u64 copy_size = Math::min(string_capacity - 1, conversion_size);
    Memory::memcpy(&string[*string_size], buffer_str, copy_size);
    *string_size += copy_size;
    string[*string_size] = u8'\0';
  } else if constexpr (IS_VEC2(T))  {
    const utf8* decorate = u8"{ x: , y: }";
    utf8 buffer[32];
    u64 size_added = 0;
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.x, 0LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.y, 5LLU, 5LLU);
    __PATHLIB_DECORATE_CLIP(9LLU, 2LLU);
    *string_size += size_added;
    string[*string_size] = u8'\0';
  } else if constexpr (IS_VEC3(T))  {
    const utf8* decorate = u8"{ x: , y: , z: }";
    utf8 buffer[32];
    u64 size_added = 0;
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.x, 0LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.y, 5LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.z, 10LLU, 5LLU);
    __PATHLIB_DECORATE_CLIP(14LLU, 2LLU);
    *string_size += size_added;
    string[*string_size] = u8'\0';
  } else if constexpr (IS_VEC4(T))  {
    const utf8* decorate = u8"{ x: , y: , z: , w: }";
    utf8 buffer[32];
    u64 size_added = 0;
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.x, 0LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.y, 5LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.z, 10LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.w, 15LLU, 5LLU);
    __PATHLIB_DECORATE_CLIP(19LLU, 2LLU);
    *string_size += size_added;
    string[*string_size] = u8'\0';
  } else if constexpr (IS_VEC8(T))  {
    const utf8* decorate = u8"{ lo: { x: , y: , z: , w: }, hi: ";
    utf8 buffer[32];
    u64 size_added = 0;
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.lo.x, 0LLU, 11LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.lo.y, 11LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.lo.z, 16LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.lo.w, 21LLU, 5LLU);
    __PATHLIB_DECORATE_CLIP(25LLU, 8LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.hi.x, 6LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.hi.y, 11LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.hi.z, 16LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_CLIP(arg.hi.w, 21LLU, 5LLU);
    __PATHLIB_DECORATE_CLIP(25LLU, 2LLU);
    *string_size += size_added;
    string[*string_size] = u8'\0';
  } else {
    static_assert(return_false<T>::value, "Unsupported type used for formatting a string.");
  }
}

//---
template <typename T>
static inline void from_type_grow(const T arg,
                                  utf8** string,
                                  u64* string_size,
                                  u64* string_capacity)
{
  static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with 'u8' for utf-8 encoding: 'u8\"Hello world!\"'");
  static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
  if constexpr (SAME_TYPE(T, utf8)) {
    u64 new_size = *string_size + 1;
    if (new_size > *string_capacity) {
      *string_capacity = new_size * 1.5;
      *string = (utf8*)REALLOC(*string, *string_capacity + 1);
    }
    (*string)[*string_size] = arg;
    (*string)[new_size] = u8'\0';
    *string_size = new_size;
  } else if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
    u64 arg_size = String::size_of(arg);
    u64 new_size = *string_size + arg_size;
    if (new_size > *string_capacity) {
      *string_capacity = new_size * 1.5;
      *string = (utf8*)REALLOC(*string, *string_capacity + 1);
    }
    Memory::memcpy(&(*string)[*string_size], arg, arg_size);
    (*string)[new_size] = u8'\0';
    *string_size = new_size;
  } else if constexpr (IS_INTEGRAL(T) || IS_FLOAT(T)) {
    utf8 buffer[32];
    u64 conversion_size;
    utf8* buffer_str = from_number(arg, buffer, &conversion_size);
    u64 new_size = *string_size + conversion_size;
    if (new_size >= *string_capacity) {
      *string_capacity = new_size * 1.5;
      *string = (utf8*)REALLOC(*string, *string_capacity + 1);
    }
    Memory::memcpy(&(*string)[*string_size], buffer_str, conversion_size);
    (*string)[new_size] = u8'\0';
    *string_size = new_size;
  } else if constexpr (IS_VEC2(T))  {
    const utf8* decorate = u8"{ x: , y: }";
    utf8 buffer[32];
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_GROW(arg.x, 0LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.y, 5LLU, 5LLU);
    __PATHLIB_DECORATE_GROW(9LLU, 2LLU);
    (*string)[*string_size] = u8'\0';
  } else if constexpr (IS_VEC3(T))  {
    const utf8* decorate = u8"{ x: , y: , z: }";
    utf8 buffer[32];
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_GROW(arg.x, 0LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.y, 5LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.z, 10LLU, 5LLU);
    __PATHLIB_DECORATE_GROW(14LLU, 2LLU);
    (*string)[*string_size] = u8'\0';
  } else if constexpr (IS_VEC4(T))  {
    const utf8* decorate = u8"{ x: , y: , z: , w: }";
    utf8 buffer[32];
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_GROW(arg.x, 0LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.y, 5LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.z, 10LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.w, 15LLU, 5LLU);
    __PATHLIB_DECORATE_GROW(19LLU, 2LLU);
    (*string)[*string_size] = u8'\0';
  } else if constexpr (IS_VEC8(T))  {
    const utf8* decorate = u8"{ lo: { x: , y: , z: , w: }, hi: ";
    utf8 buffer[32];
    u64 conversion_size;
    __PATHLIB_DECORATE_NUMBER_GROW(arg.lo.x, 0LLU, 11LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.lo.y, 11LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.lo.z, 16LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.lo.w, 21LLU, 5LLU);
    __PATHLIB_DECORATE_GROW(25LLU, 8LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.hi.x, 6LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.hi.y, 11LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.hi.z, 16LLU, 5LLU);
    __PATHLIB_DECORATE_NUMBER_GROW(arg.hi.w, 21LLU, 5LLU);
    __PATHLIB_DECORATE_GROW(25LLU, 2LLU);
    (*string)[*string_size] = u8'\0';
  } else {
    static_assert(return_false<T>::value, "Unsupported type used for formatting a string.");
  }
}
}