#pragma once

#if defined(_MSC_VER)
  #include <immintrin.h>
#else
  #include "mmintrin.h"
  #include "xmmintrin.h"
  #include "avxintrin.h"
  #include "avx2intrin.h"
  #include "bmiintrin.h"
#endif
