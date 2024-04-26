#pragma once

#if defined(_MSC_VER)
  #include <immintrin.h>
#else
  #include "std/intrinsics/mmintrin.h"
  #include "std/intrinsics/xmmintrin.h"
  #include "std/intrinsics/avxintrin.h"
  #include "std/intrinsics/avx2intrin.h"
  #include "std/intrinsics/bmiintrin.h"
#endif
