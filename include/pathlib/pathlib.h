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
#include "pathlib/containers/safe_ptr.h"
#include "pathlib/containers/array.h"
#include "pathlib/containers/array_unsafe.h"
#include "pathlib/containers/vector.h"
#include "pathlib/containers/vector_unsafe.h"
#include "pathlib/containers/hashmap.h"
#include "pathlib/containers/hashmap_unsafe.h"
#include "pathlib/containers/pool.h"
#include "pathlib/containers/pool_unsafe.h"
#include "pathlib/containers/pools.h"
#include "pathlib/containers/pools_unsafe.h"
#include "pathlib/string/compare.h"
#include "pathlib/string/from_type.h"
#include "pathlib/string/long_string.h"
#include "pathlib/string/long_string_unsafe.h"
#include "pathlib/string/short_string.h"
#include "pathlib/string/short_string_unsafe.h"
#include "pathlib/string/size_of.h"
#include "pathlib/profiler/profiler.h"
#include "pathlib/timer/timer.h"

using namespace Pathlib;

#define PATHLIB_LOG_PATH u8"pathlib_log.txt"