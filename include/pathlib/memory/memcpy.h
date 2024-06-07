/*
  Documentation: https://www.path.blog/docs/memcpy.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memory.h"
#include "pathlib/math/math.h"
#include "pathlib/math/simd_math.h"
#include "pathlib/containers/safe_ptr.h"

namespace Pathlib {
namespace _Internal {

//---
template <typename T,
          bool DST_ALIGNED_32 = false,
          bool SRC_ALIGNED_32 = false>
static inline void memcpy(SafePtr<T> dst,
                          SafePtr<T> src,
                          u64 count)
{
  T* dst_ptr = dst;
  T* src_ptr = src;
  if (EXPECT(((dst_ptr + count) >= dst_ptr) &&
             ((src_ptr + count) >= src_ptr) &&
             ((src_ptr + count) <= (src_ptr + src.get_count())) &&
             ((dst_ptr + count) <= (dst_ptr + dst.get_count())))) {
    memcpy_unsafe(dst_ptr, src_ptr, count * sizeof(T));
  } else {
    get_errors().to_log(u8"Out of bounds memcpy().");
    get_errors().kill_script();
  }
}
}