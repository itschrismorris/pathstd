/*
  Documentation: https://www.path.blog/docs/math.html
*/

#pragma once
#include "types.h"
#include "intrinsics/intrinsics.h"

namespace Pathlib::Math {

  /**/
  template <typename T>
  static inline constexpr T min(T a, T b)
  { 
    return (a < b) ? a : b; 
  }

  /**/
  template <typename T>
  static inline constexpr T max(T a, T b)
  { 
    return (a > b) ? a : b;
  }

  /**/
  template <typename T>
  static inline constexpr T abs(T value)
  {
    if constexpr (SAME_TYPE(T, i32) || SAME_TYPE(T, i16) || SAME_TYPE(T, i8)) {
      return __builtin_abs(value);
    } else if constexpr (SAME_TYPE(T, i64)) {
      return __builtin_llabs(value);
    } else if constexpr (SAME_TYPE(T, f32)) {
      return __builtin_fabsf(value);
    } else if constexpr (SAME_TYPE(T, f64)) {
      return __builtin_fabs(value);
    } else {
      return value;
    }
  }

  /**/
  template <typename T>
  static inline constexpr bool is_inf(T value)
  {
    if constexpr (SAME_TYPE(T, f32)) {
      return (((*(u32*)&value) & 0x7FFFFFFF) == 0x7F800000);
    } else if constexpr (SAME_TYPE(T, f64)) {
      return (((*(u64*)&value) & 0x7FFFFFFFFFFFFFFFLLU) == 0x7FF0000000000000LLU);
    }
  }

  /**/
  template <typename T>
  static inline constexpr bool is_nan(T value)
  {
    if constexpr (SAME_TYPE(T, f32)) {
      return (value != value);
    } else if constexpr (SAME_TYPE(T, f64)) {
      return (value != value);
    }
  }
   
  /**/
  template <typename T>
  static inline constexpr bool is_pot(T value)
  { 
    return ((value & (value + (value == 0) - 1)) == 0);
  }

  /**/
  template <typename T, T MULTIPLE>
  static inline constexpr bool is_multiple_of(T value)
  {
    if constexpr (Math::is_pot(MULTIPLE)) {
      return ((value & (MULTIPLE - 1)) == 0);
    } else {
      return ((value % MULTIPLE) == 0);
    }
  }

  /**/
  template <typename T, T MULTIPLE>
  static inline constexpr T previous_multiple_of(T value)
  { 
    if constexpr (Math::is_pot(MULTIPLE)) {
      return (value & ~(MULTIPLE - 1));
    } else {
      return (value - (value % MULTIPLE));
    }
  }

  /**/
  template <typename T, T MULTIPLE>
  static inline constexpr T next_multiple_of(T value)
  {
    if constexpr (Math::is_pot(MULTIPLE)) {
      return (value + (value == 0) + (MULTIPLE - 1)) & -MULTIPLE;
    } else {
      return ((value + (value == 0) + (MULTIPLE - 1)) / MULTIPLE) * MULTIPLE;
    }
  }
  
  /**/
  template <typename T>
  static inline constexpr T next_pot(T value)
  {
    if constexpr (sizeof(T) == 4) {
      return (0x1 << (32 - __builtin_clz(value + (value == 0))));
    } else if constexpr (sizeof(T) == 8) {
      return (0x1LLU << (64 - __builtin_clzll(value + (value == 0))));
    }
  }

  /**/
  template <typename T>
  static inline T lsb_set(T value)
  {
    if constexpr (sizeof(T) == 2) {
      return __builtin_ia32_tzcnt_u16(value);
    } else if constexpr (sizeof(T) == 4) {
      return __builtin_ia32_tzcnt_u32(value);
    } else if constexpr (sizeof(T) == 8) {
      return __builtin_ia32_tzcnt_u64(value);
    }
  }

  /**/
  template <typename T>
  static inline T msb_set(T value)
  {
    if constexpr (sizeof(T) == 2) {
      return __builtin_ia32_lzcnt_u16(value);
    } else if constexpr (sizeof(T) == 4) {
      return __builtin_ia32_lzcnt_u32(value);
    } else if constexpr (sizeof(T) == 8) {
      return __builtin_ia32_lzcnt_u64(value);
    }
  }
}