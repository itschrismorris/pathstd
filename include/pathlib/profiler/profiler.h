/*
  Documentation: https://www.path.blog/docs/profile.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/containers/pools_unsafe.h"
#include "pathlib/string/fixed_string_unsafe.h"
#include "pathlib/containers/vector_unsafe.h"
#include "../src/third_party/mimalloc/mimalloc.h"

namespace Pathlib::_Internal {

//---
struct Profiler
{
  //---
  Profiler();
  ~Profiler() {}

  //---
  void list_memory(VectorUnsafe<FixedStringUnsafe<64>, 128>& names_out,
                   VectorUnsafe<u64, 128>& sizes_out);
};
}

//---
namespace Pathlib { _Internal::Profiler& get_profiler(); }