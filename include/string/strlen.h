/* 
  Documentation: https://www.path.blog/docs/strlen.html
*/

#pragma once
#include "types.h"
#include "math/math.h"

namespace Pathlib::String {

/**/
template <u32 ALIGNED_32 = false, 
          u64 MAX_LENGTH = U64_MAX>
static inline u64 strlen(const utf8* str)
{
  I8 zero = I8_SETZERO();
  if ((MAX_LENGTH <= 512) && (ALIGNED_32 || Math::is_aligned<32>(str))) {
    const I8* str_v = (I8*)str;
    constexpr u32 register_count = (Math::next_multiple_of<32>(MAX_LENGTH) >> 5);
    #pragma unroll
    for (u32 r = 0; r < register_count; ++r) {
      u32 mask = I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD(str_v), zero));
      if (mask) {
        return (((r << 5) + Math::lsb_set(mask)));
      }
      str_v += 1;
    }
  } else {
    const I8* str_v = (I8*)Math::align_previous<64>(str);
    i64 ignore_bytes = (u8*)str - (u8*)str_v;
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
        return (((u8*)str_v - (u8*)str) + Math::lsb_set(zero_mask_64));
      }
      str_v += 2;
    }
  }
  return 0;
}
}