/* 'memory/avx_memcpy.h'

  + Invokes hand-written assembly that utilizes AVX to copy from src to dst.
  + Protected against page boundary segfaults.
  + Fast path for AVX-aligned src and dst.
  + Copies a cache-line (64B) per iteration.

    Path game engine: https://www.path.blog
*/

#pragma once
#include "memory/memory.h"
#include "math/math.h"

extern "C" { void asm_memcpy_aligned(u8* dst, const u8* src, u64 size);
             void asm_memcpy_unaligned(u8* dst, const u8* src, u64 size); }

namespace Pathlib::Memory {


template <bool DST_AVX_ALIGNED = false,
          bool SRC_AVX_ALIGNED = false>
static inline void memcpy(u8* dst, 
                          const u8* src,
                          u64 size)
{
  if ((DST_AVX_ALIGNED || Math::is_aligned<32>(dst)) &&
      (SRC_AVX_ALIGNED || Math::is_aligned<32>(src))) {
    asm_memcpy_aligned(dst, src, size);
  } else {
    asm_memcpy_unaligned(dst, src, size);
  }
}

/**/
template <bool DST_AVX_ALIGNED = false, bool SRC_AVX_ALIGNED = false>
static void avx_memcpy(u8* dst, 
                       const u8* src,
                       u64 size)
{
  if (size <= 128) {
    memcpy(dst, src, size);
    return;
  }
  const I8* src_v = (const I8*)src;
  I8* dst_v = (I8*)dst;
  if (!(DST_AVX_ALIGNED || Math::is_aligned<32>(dst))) {
    u64 padding = (32 - (((size_t)dst) & 31)) & 31;
    I8_STOREU(dst_v, I8_LOADU(src_v));
    dst_v = (I8*)(dst + padding);
    src_v = (I8*)(src + padding);
    size -= padding;
  }
  u32 loop_count = (size >> 6);
  if (size <= MEGABYTE) {
    if (SRC_AVX_ALIGNED || Math::is_aligned<32>(src_v)) {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 a = I8_LOAD(src_v);
        I8 b = I8_LOAD(src_v + 1);
        I8_STORE(dst_v, a);
        I8_STORE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    } else {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 a = I8_LOADU(src_v);
        I8 b = I8_LOADU(src_v + 1);
        I8_STORE(dst_v, a);
        I8_STORE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    }
  } else {
    if (SRC_AVX_ALIGNED || Math::is_aligned<32>(src_v)) {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 a = I8_LOAD(src_v);
        I8 b = I8_LOAD(src_v + 1);
        I8_STORE_NOCACHE(dst_v, a);
        I8_STORE_NOCACHE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    } else {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 a = I8_LOADU(src_v);
        I8 b = I8_LOADU(src_v + 1);
        I8_STORE_NOCACHE(dst_v, a);
        I8_STORE_NOCACHE(dst_v + 1, b);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    }
    FENCE();
  }
  if (size > 0) {
    src = (u8*)src_v;
    dst = (u8*)dst_v;
    memcpy(dst, src, size);
  }
}
}