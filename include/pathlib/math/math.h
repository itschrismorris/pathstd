/*
  Documentation: https://www.path.blog/docs/math.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/math/intrinsics/intrinsics.h"

namespace Pathlib::Math {

//---
template <typename T>
static inline constexpr T min(T a, T b)
{ 
  return (a < b) ? a : b; 
}

//---
template <typename T>
static inline constexpr T max(T a, T b)
{ 
  return (a > b) ? a : b;
}

//---
template <typename T>
static inline i32 lsb_set(T value)
{
  if constexpr (sizeof(T) == 2) {
    return (value == 0) ? -1 : __builtin_ia32_tzcnt_u16(value);
  } else if constexpr (sizeof(T) == 4) {
    return (value == 0) ? -1 : __builtin_ia32_tzcnt_u32(value);
  } else if constexpr (sizeof(T) == 8) {
    return (value == 0) ? -1 : __builtin_ia32_tzcnt_u64(value);
  }
}

//---
template <typename T>
static inline i32 msb_set(T value)
{
  if constexpr (sizeof(T) == 2) {
    return (15 - __builtin_ia32_lzcnt_u16(value));
  } else if constexpr (sizeof(T) == 4) {
    return (31 - __builtin_ia32_lzcnt_u32(value));
  } else if constexpr (sizeof(T) == 8) {
    return (63 - __builtin_ia32_lzcnt_u64(value));
  }
}

//---
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

//---
template <typename T>
static inline constexpr T log2(T value)
{
  return msb_set(value);
}

//---
template <typename T>
static inline constexpr bool is_inf(T value)
{
  if constexpr (SAME_TYPE(T, f32)) {
    return (((*(u32*)&value) & 0x7FFFFFFF) == 0x7F800000);
  } else if constexpr (SAME_TYPE(T, f64)) {
    return (((*(u64*)&value) & 0x7FFFFFFFFFFFFFFFLLU) == 0x7FF0000000000000LLU);
  }
}

//---
template <typename T>
static inline constexpr bool is_nan(T value)
{
  if constexpr (SAME_TYPE(T, f32)) {
    return (value != value);
  } else if constexpr (SAME_TYPE(T, f64)) {
    return (value != value);
  }
}
   
//---
template <typename T>
static inline constexpr bool is_pot(T value)
{
  if (value <= 1) return false;
  return ((value & (value - 1)) == 0);
}

//---
template <typename T, 
          T MULTIPLE>
static inline constexpr bool is_multiple_of(T value)
{
  if constexpr (Math::is_pot(MULTIPLE)) {
    return ((value & (MULTIPLE - 1)) == 0);
  } else {
    return ((value % MULTIPLE) == 0);
  }
}

//---
template <typename T, 
          T MULTIPLE>
static inline constexpr T previous_multiple_of(T value)
{ 
  if constexpr (Math::is_pot(MULTIPLE)) {
    return (value & ~(MULTIPLE - 1));
  } else {
    return (value - (value % MULTIPLE));
  }
}

//---
template <typename T, 
          T MULTIPLE>
static inline constexpr T next_multiple_of(T value)
{
  if constexpr (Math::is_pot(MULTIPLE)) {
    return (value + (value == 0) + (MULTIPLE - 1)) & -MULTIPLE;
  } else {
    return ((value + (value == 0) + (MULTIPLE - 1)) / MULTIPLE) * MULTIPLE;
  }
}
  
//---
template <typename T>
static inline constexpr T round_down_to_pot(T value)
{
  if (is_pot(value)) return value;
  if (value <= 1) return 2;
  if constexpr (sizeof(T) == 2) {
    return (0x1 << (15 - __builtin_ia32_lzcnt_u16(value)));
  } else if constexpr (sizeof(T) == 4) {
    return (0x1 << (31 - __builtin_ia32_lzcnt_u32(value)));
  } else if constexpr (sizeof(T) == 8) {
    return (0x1LLU << (63 - __builtin_ia32_lzcnt_u64(value)));
  }
}
  
//---
template <typename T>
static inline constexpr T round_up_to_pot(T value)
{
  if (is_pot(value)) return value;
  if (value <= 1) return 2;
  if constexpr (sizeof(T) == 2) {
    return (0x1 << (16 - __builtin_ia32_lzcnt_u16(value)));
  } else if constexpr (sizeof(T) == 4) {
    return (0x1 << (32 - __builtin_ia32_lzcnt_u32(value)));
  } else if constexpr (sizeof(T) == 8) {
    return (0x1LLU << (64 - __builtin_ia32_lzcnt_u64(value)));
  }
}

static inline u32 hash(u32 value)
{
  ++value;
  value ^= value >> 16;
  value *= 0x21f0aaad;
  value ^= value >> 15;
  value *= 0xd35a2d97;
  value ^= value >> 15;
  return value;
}

//---
static inline u32 reverse_hash(u32 value)
{
  value ^= value >> 15;
  value ^= value >> 30;
  value *= 0x37132227;
  value ^= value >> 15;
  value ^= value >> 30;
  value *= 0x333c4925;
  value ^= value >> 16;
  return --value;
}

//---
static inline u32 hash(u64 value)
{
  return (hash((u32)(value & 0x00000000FFFFFFFFLLU)) +
          hash((u32)(value >> 32)));
}

//---
static inline u32 hash(f32 f)
{
  u32 value = *(u32*)&f;
  return hash(value);
}

//---
static inline u32 hash(f64 d)
{
  u64 value = *(u64*)&d;
  return hash(value);
}

//---
template <typename T>
static inline u32 hash(T* key,
                       u64 size)
{
  u32 h = 0;
  u32 passes = (size >> 2);
  for (u32 p = 0; p < passes; ++p) {
    h += hash(((u32*)key)[p]);
  }
  u32 leftover = size - (passes << 2);
  if (leftover) {
    key = (T*)((u32*)key + passes);
    for (u32 b = 0; b < leftover; ++b) {
      h += hash((u32)((u8*)key)[b]);
    }
  }
  return h;
}
}