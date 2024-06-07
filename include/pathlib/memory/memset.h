/*
  Documentation: https://www.path.blog/docs/memset.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/math/math.h"
#include "pathlib/math/simd_math.h"
#include "pathlib/memory/memory.h"
#include "pathlib/containers/safe_ptr.h"

namespace Pathlib {

//---
template <typename T,
          bool DST_ALIGNED_32 = false,
          bool SRC_ALIGNED_32 = false>
static inline void memset(SafePtr<T> dst,
                          const u8 value,
                          u64 count)
{
  T* dst_ptr = dst;
  if (EXPECT(((dst_ptr + count) >= dst_ptr) &&
             ((dst_ptr + count) <= (dst_ptr + dst.get_count())))) {
    memset_unsafe(dst_ptr, value, count * sizeof(T));
  } else {
    get_errors().to_log_with_stacktrace(u8"Out of bounds memset().");
    get_errors().kill_script();
  }
}
}