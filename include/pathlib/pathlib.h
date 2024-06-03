/*
  Documentation: https://www.path.blog/docs/pathlib.html
*/

#pragma once
#include "pathlib/win32/console.h"
#include "pathlib/errors/errors.h"
#include "pathlib/log/log.h"
#include "pathlib/math/math.h"
#include "pathlib/memory/malloc.h"
#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/memory/memcpy.h"
#include "pathlib/memory/memset.h"
#include "pathlib/memory/memory.h"
#include "pathlib/types/containers/safe_ptr.h"
#include "pathlib/types/containers/array.h"
#include "pathlib/types/containers/array_unsafe.h"
#include "pathlib/types/containers/vector.h"
#include "pathlib/types/containers/vector_unsafe.h"
#include "pathlib/types/containers/hashmap.h"
#include "pathlib/types/containers/hashmap_unsafe.h"
#include "pathlib/types/containers/pool.h"
#include "pathlib/types/containers/pool_unsafe.h"
#include "pathlib/types/containers/pools.h"
#include "pathlib/types/containers/pools_unsafe.h"
#include "pathlib/types/string/compare.h"
#include "pathlib/types/string/from_type.h"
#include "pathlib/types/string/long_string.h"
#include "pathlib/types/string/long_string_unsafe.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/types/string/short_string_unsafe.h"
#include "pathlib/types/string/size_of.h"
#include "pathlib/profiler/profiler.h"
#include "pathlib/timer/timer.h"

using namespace Pathlib;

#define PATHLIB_LOG_PATH u8"pathlib_log.txt"