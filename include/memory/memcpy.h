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

extern "C" { void asm_memcpy(u8* dst, const u8* src, u64 size); }

namespace Pathlib::Memory {

/**/
template <bool DST_AVX_ALIGNED = false,
          bool SRC_AVX_ALIGNED = false>
static inline void memcpy(u8* dst, 
                          const u8* src,
                          u64 size)
{
  if ((DST_AVX_ALIGNED || Math::is_aligned<32>(dst)) &&
      (SRC_AVX_ALIGNED || Math::is_aligned<32>(src))) {
    //avx_memcpy_aligned_asm((void*)dst, (const void*)src, (size_t)size);
  } else {
    asm_memcpy(dst, src, size);
  }
}
}