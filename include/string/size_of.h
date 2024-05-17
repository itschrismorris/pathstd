/* 
  Documentation: https://www.path.blog/docs/size.html
*/

#pragma once
#include "types.h"
#include "math/math.h"

namespace Pathlib::String {

/**/
template <u32 ALIGNED_32 = false, 
          u64 MAX_LENGTH = U64_MAX,
          typename T>
static inline u64 size_of(const T arg)
{
  static_assert(!SAME_TYPE(T, const char*), "String literals must be prepended with 'u8' for utf-8 encoding: 'u8\"Hello world!\"'");
  static_assert(!SAME_TYPE(T, char*), "Replace string usages of char with utf8, for utf-8 encoding.");
  if constexpr (SAME_TYPE(T, const utf8*) || SAME_TYPE(T, utf8*)) {
    I8 zero = I8_SETZERO();
    if ((MAX_LENGTH <= 512) && (ALIGNED_32 || Math::is_aligned<32>(arg))) {
      const I8* str_v = (I8*)arg;
      constexpr u32 register_count = (Math::next_multiple_of<u64, 32>(MAX_LENGTH) >> 5);
      #pragma unroll
      for (u32 r = 0; r < register_count; ++r) {
        u32 mask = I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD(str_v), zero));
        if (mask) {
          return (((r << 5) + Math::lsb_set(mask)));
        }
        str_v += 1;
      }
    } else {
      const I8* str_v = (I8*)Math::align_previous<64>(arg);
      i64 ignore_bytes = (u8*)arg - (u8*)str_v;
      u64 mask32 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD(str_v), zero));
      u64 mask64 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD(str_v + 1), zero));
      u64 zero_mask_64 = (mask32 | (mask64 << 32)) & (U64_MAX << ignore_bytes);
      if (zero_mask_64) {
        return (Math::lsb_set(zero_mask_64) - ignore_bytes);
      }
      str_v += 2;
      while (true) {
        u64 mask32 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD(str_v), zero));
        u64 mask64 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD(str_v + 1), zero));
        u64 zero_mask_64 = (mask32 | (mask64 << 32));
        if (zero_mask_64) {
          return (((u8*)str_v - (u8*)arg) + Math::lsb_set(zero_mask_64));
        }
        str_v += 2;
      }
    }
  } else if constexpr (IS_INTEGRAL(T) || IS_FLOAT(T)) {
    utf8 buffer[32];
    u64 size;
    _from_number(arg, buffer, &size);
    return size;
  } else if constexpr (IS_VEC2(T))  {
    utf8 buffer[32];
    u64 size;
    u64 conversion_size;
    _from_number(arg.x, buffer, &conversion_size);
    size = conversion_size;
    _from_number(arg.y, buffer, &conversion_size);
    size += conversion_size;
    size += 12;
    return size;
  } else if constexpr (IS_VEC3(T))  {
    utf8 buffer[32];
    u64 size;
    u64 conversion_size;
    _from_number(arg.x, buffer, &conversion_size);
    size = conversion_size;
    _from_number(arg.y, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.z, buffer, &conversion_size);
    size += conversion_size;
    size += 17;
    return size;
  } else if constexpr (IS_VEC4(T))  {
    utf8 buffer[32];
    u64 size;
    u64 conversion_size;
    _from_number(arg.x, buffer, &conversion_size);
    size = conversion_size;
    _from_number(arg.y, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.z, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.w, buffer, &conversion_size);
    size += conversion_size;
    size += 22;
    return size;
  } else if constexpr (IS_VEC8(T))  {
    utf8 buffer[32];
    u64 size;
    u64 conversion_size;
    _from_number(arg.lo.x, buffer, &conversion_size);
    size = conversion_size;
    _from_number(arg.lo.y, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.lo.z, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.lo.w, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.hi.x, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.hi.y, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.hi.z, buffer, &conversion_size);
    size += conversion_size;
    _from_number(arg.hi.w, buffer, &conversion_size);
    size += 56;
    return size;
  } else {
    static_assert(false, "Unsupported type used for getting size of a string.");
  }
  return 0;
}
}