#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/profiler/profiler.h"
#include "pathlib/containers/pool_unsafe.h"
#include "../src/third_party/mimalloc/mimalloc.h"

#define NOT_TRACKED 0xFFFFFFFF

namespace Pathlib {

//---
void* malloc_unsafe(u64 size,
                    const utf8* name)
{
  u64 good_size = mi_malloc_good_size(size + sizeof(u32));
  u8* ptr = (u8*)mi_malloc_aligned(good_size, 128);
  if (EXPECT(name != nullptr)) {
    _Internal::Profiler::MemoryItem* memory_item = get_profiler()._memory_items.get_vacant();
    *(u32*)(&ptr[good_size - sizeof(u32)]) = memory_item->_pool_id;
    memory_item->_name = name;
    memory_item->_size = good_size;
  } else {
    *(u32*)(&ptr[good_size - sizeof(u32)]) = NOT_TRACKED;
  }
  return ptr;
}

//---
void* realloc_unsafe(void* ptr,
                     u64 size)
{
  u64 previous_size = mi_usable_size(ptr);
  u32 pool_id = *(u32*)(&((u8*)ptr)[previous_size - sizeof(u32)]);
  u64 good_size = mi_malloc_good_size(size + sizeof(u32));
  u8* new_ptr = (u8*)mi_realloc_aligned(ptr, good_size, 128);
  if (EXPECT(pool_id != NOT_TRACKED)) {
    _Internal::Profiler::MemoryItem* memory_item = get_profiler()._memory_items[pool_id];
    memory_item->_size = good_size;
    *(u32*)(&((u8*)new_ptr)[good_size - sizeof(u32)]) = memory_item->_pool_id;
  } else {
    *(u32*)(&((u8*)new_ptr)[good_size - sizeof(u32)]) = NOT_TRACKED;
  }
  return new_ptr;
}

//---
void free_unsafe(void** ptr)
{
  u64 previous_size = mi_usable_size(*ptr);
  u32 pool_id = *(u32*)(&((u8*)(*ptr))[previous_size - sizeof(u32)]);
  if (EXPECT(pool_id != NOT_TRACKED)) {
    get_profiler()._memory_items.free(pool_id);
  }
  mi_free_aligned(*ptr, 128);
  *ptr = nullptr;
}
}