#pragma once
#if defined(_MSC_VER)
	#include <immintrin.h>
#else
	#include "pathlib/math/intrinsics/mmintrin.h"
	#include "pathlib/math/intrinsics/xmmintrin.h"
	#include "pathlib/math/intrinsics/avxintrin.h"
	#include "pathlib/math/intrinsics/avx2intrin.h"
	#include "pathlib/math/intrinsics/bmiintrin.h"
#endif
