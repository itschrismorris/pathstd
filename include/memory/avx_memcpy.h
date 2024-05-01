/* 'std/string/avx_memcpy.h'

  + Utilizes AVX to copy data from src to dst.
  + Protected against page boundary segfaults.
  + Fast path for AVX-aligned src and/or dst.
  + Copies a cache-line (64B) per iteration.

    Path game engine: https://www.path.blog
*/

#pragma once
#include "memory/memory.h"
#include "math/math.h"

namespace Pathlib::Memory {

/**/
template <bool DST_AVX_ALIGNED = false,
          bool SRC_AVX_ALIGNED = false>
static inline void avx_memcpy(u8* dst, 
                              const u8* src,
                              u64 size)
{
  if (size < 32) {
    for (u32 b = 0; b < size; ++b) {
      dst[b] = src[b];
    }
    return;
  }
  const I8* src_v = (const I8*)src;
  I8* dst_v = (I8*)dst;
  if ((DST_AVX_ALIGNED || Math::is_aligned<32>(dst)) && 
      (SRC_AVX_ALIGNED || Math::is_aligned<32>(src))) {
    u32 register_count = (size >> 5);
    if (size <= MEGABYTE) {
      for (u32 r = 0; r < register_count; r += 2) {
        I8 a = I8_LOAD(src_v);
        I8 b = I8_LOAD(src_v + 1);
        I8_STORE(dst_v, a);
        I8_STORE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
      }
    } else {
      for (u32 r = 0; r < register_count; r += 2) {
        I8 a = I8_LOAD(src_v);
        I8 b = I8_LOAD(src_v + 1);
        I8_STORE_NOCACHE(dst_v, a);
        I8_STORE_NOCACHE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
      }
      FENCE();
    }
  } else {
    u64 padding = (32 - (((u64)dst) & 31)) & 31;
    I8_STOREU(dst_v, I8_LOADU(src_v));
    dst_v = (I8*)(dst + padding);
    src_v = (I8*)(src + padding);
    u32 register_count = ((size - padding) >> 5);
    if (size <= MEGABYTE) {
      for (u32 r = 0; r < register_count; r += 2) {
        I8 a = I8_LOADU(src_v);
        I8 b = I8_LOADU(src_v + 1);
        I8_STORE(dst_v, a);
        I8_STORE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
      }
    } else {
      for (u32 r = 0; r < register_count; r += 2) {
        I8 a = I8_LOADU(src_v);
        I8 b = I8_LOADU(src_v + 1);
        I8_STORE_NOCACHE(dst_v, a);
        I8_STORE_NOCACHE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
      }
      FENCE();
    }
  }
  dst_v = (I8*)(dst + size - 64);
  src_v = (I8*)(src + size - 64);
  I8 a = I8_LOADU(src_v);
  I8 b = I8_LOADU(src_v + 1);
  I8_STOREU(dst_v, a);
  I8_STOREU(dst_v + 1, b);
}
}