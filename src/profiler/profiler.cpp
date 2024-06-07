#include "pathlib/pathlib.h"
#include "../src/third_party/mimalloc/mimalloc.h"

namespace Pathlib {
//---
_Internal::Profiler& get_profiler() 
{
  static _Internal::Profiler profiler;
  return profiler;
}
}

namespace Pathlib::_Internal {

//---
Profiler::Profiler()
{
  #ifdef _DEBUG
    mi_option_enable(mi_option_verbose);
  #endif
}

//---
static bool list_memory_callback(const mi_heap_t* heap, 
                                 const mi_heap_area_t* area, 
                                 void* block, 
                                 size_t block_size, 
                                 void* arg) 
{
  void** out = (void**)arg;
  VectorUnsafe<FixedStringUnsafe<64>, 128>* names_out = (VectorUnsafe<FixedStringUnsafe<64>, 128>*)out[0];
  VectorUnsafe<u64, 128>* sizes_out = (VectorUnsafe<u64, 128>*)out[1];
  
  if (block && (block_size > 64)) {
    void* aligned_ptr = Math::align_next<64>(block);
    utf8 name[64];
    memcpy_unsafe(name, aligned_ptr, 64);
    name[63] = u8'\0';
    *names_out->emplace_back(1) = name;
    *sizes_out->emplace_back(1) = block_size;
  }
  return true;
}

//---
void Profiler::list_memory(VectorUnsafe<FixedStringUnsafe<64>, 128>& names_out,
                           VectorUnsafe<u64, 128>& sizes_out)
{
  void* out[2] = { &names_out, &sizes_out };
  mi_heap_visit_blocks(mi_heap_get_default(), true, list_memory_callback, out);
}
}