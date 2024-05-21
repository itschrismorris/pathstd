#pragma once
#include "error/error.h"
#include "log/log.h"
#include "math/math.h"
#include "memory/malloc.h"
#include "memory/memcpy.h"
#include "memory/memset.h"
#include "types/containers/short_vector.h"
#include "types/containers/long_vector.h"
#include "types/containers/hashmap.h"
#include "types/string/compare.h"
#include "types/string/from_type.h"
#include "types/string/long_string.h"
#include "types/string/short_string.h"
#include "types/string/size_of.h"
#include "profile/profile.h"
#include "timer/timer.h"

namespace Pathlib {

/**/
bool initiate(const utf8* log_path = nullptr);
void shutdown();
}