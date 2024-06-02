#include "pathlib/memory/malloc_unsafe.h"
#include "pathlib/profile/profile.h"
#include "../src/third_party/mimalloc/mimalloc.h"

namespace Pathlib {

//---
void* malloc_unsafe(u64 size,
                    const utf8* name)
{
  if (name) {
    u64 good_size = mi_malloc_good_size(size + sizeof(u32));
    u8* ptr = (u8*)mi_malloc_aligned(good_size, 128);
    _Internal::Profile::MemoryItem* memory_item = profile.memory_items.get_vacant();
    *(u32*)(&ptr[good_size - sizeof(u32)]) = memory_item->pool_id;
    memory_item->name = name;
    memory_item->size = good_size;
    return ptr;
  } else {
    return mi_malloc_aligned(size, 128);
  }
}

//---
void* realloc_unsafe(void* ptr,
                     u64 size,
                     bool named)
{
  if (named) {
    u64 previous_size = mi_usable_size(ptr);
    u32 pool_id = *(u32*)(&((u8*)ptr)[previous_size - sizeof(u32)]);
    _Internal::Profile::MemoryItem* memory_item = profile.memory_items[pool_id];
    u64 good_size = mi_malloc_good_size(size + sizeof(u32));
    memory_item->size = good_size;
    u8* new_ptr = (u8*)mi_realloc_aligned(ptr, good_size, 128);
    *(u32*)(&((u8*)ptr)[good_size - sizeof(u32)]) = memory_item->pool_id;
    return new_ptr;
  } else {
    return mi_realloc_aligned(ptr, size, 128);
  }
}

//---
void free_unsafe(void** ptr,
                 bool named)
{
  if (named) {
    u64 previous_size = mi_usable_size(ptr);
    u32 pool_id = *(u32*)(&((u8*)ptr)[previous_size - sizeof(u32)]);
    profile.memory_items.free(pool_id);
  }
  mi_free_aligned(*ptr, 128);
  *ptr = nullptr;
}
}