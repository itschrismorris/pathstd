/* 'std/string/avx_strlen.h'

  + Utilizes AVX2 to read length of an aligned or unaligned string.
  + Protected against page boundary segfaults.
  + Fast path for AVX aligned strings with a known upper-bound on length.

    Path game engine: https://www.path.blog
*/

#pragma once
#include "../types.h"
#include "../math/math.h"

namespace Path::Std::String {

/**/
template <bool ALIGNED_32 = false, u64 MAX_LENGTH = UINT64_MAX>
static inline u64 avx_strlen(const char* str)
{
  I8 zero = I8_SETZERO();
  if ((MAX_LENGTH < UINT64_MAX) && (ALIGNED_32 || Math::is_aligned<32>(str))) {
    constexpr u32 register_count = (Math::next_power_of_two_multiple<32>(MAX_LENGTH) >> 5);
    #pragma unroll
    for (u32 r = 0; r < register_count; ++r) {
      u32 mask = I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD((I8*)(str + (r << 5))), zero));
      if (mask) {
        return (((r << 5) + Math::lsb_set(mask)));
      }
    }
  } else {
    const char* _s = Math::align_previous<64>(str);
    i64 ignore_bytes = str - _s;
    u64 mask32 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD((I8*)(_s)), zero));
    u64 mask64 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD((I8*)(_s + 32)), zero));
    u64 zero_mask = (mask32 | (mask64 << 32)) & (UINT64_MAX << ignore_bytes);
    if (zero_mask) {
      return Math::lsb_set(zero_mask) - ignore_bytes;
    }
    _s += 64;
    u32 previous_bytes = 64 - ignore_bytes;
    while (true) {
      u64 mask32 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD((I8*)_s), zero));
      u64 mask64 = (u32)I8_MOVEMASK(I8_CMP_EQ8(I8_LOAD((I8*)(_s + 32)), zero));
      u64 mask = (mask32 | (mask64 << 32));
      if (mask) {
        return (previous_bytes + Math::lsb_set(mask));
      }
      _s += 64;
      previous_bytes += 64;
    }
  }
  return 0;
}
}