#pragma once
#include "pathlib/error/error.h"
#include "pathlib/log/log.h"
#include "pathlib/math/math.h"
#include "pathlib/memory/malloc.h"
#include "pathlib/memory/memcpy.h"
#include "pathlib/memory/memset.h"
#include "pathlib/types/containers/short_vector.h"
#include "pathlib/types/containers/long_vector.h"
#include "pathlib/types/containers/hashmap.h"
#include "pathlib/types/string/compare.h"
#include "pathlib/types/string/from_type.h"
#include "pathlib/types/string/long_string.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/types/string/size_of.h"
#include "pathlib/profile/profile.h"
#include "pathlib/timer/timer.h"

namespace Pathlib {

/**/
bool initiate(const utf8* log_path = nullptr);
void shutdown();
}