/*
  Documentation: https://www.path.blog/docs/pool.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"

namespace Pathlib::Containers {

//---
template <typename T, 
          u16 CAPACITY>
struct Pool
{
  //---
  static_assert(CAPACITY <= Types::U16_MAX, "Pool CAPACITY cannot exceed 65535 (16-bits used for pool_id).");

  //---
  u16 count;
  u16 free_count;
  u16 free_head;
  T* data;
  
  //---
  Pool() 
  {
    count = 0;
    free_count = 1;
    free_head = 0;
    data = (T*)MALLOC(sizeof(T) * CAPACITY);
  }

  //---
  ~Pool() 
  {
    iterate([&](T* object)
      {
        Memory::call_destructor<T>(object);
        return true;
      });
    if (data) {
      FREE(data);
    }
  }

  //---
  static inline constexpr u64 capacity()
  {
    return CAPACITY;
  }

  //---
  static inline bool is_occupied(u32 id)
  {
    return (id < 0xFFFF0000);
  }

  //---
  inline T* alloc(u32 pools_id = 0)
  {
    if (count >= CAPACITY) {
      error.last_error.format(u8"Failed to alloc() from pool; it is already at capacity.");
      error.to_log();
      return nullptr;
    }
    ++count;
    T* new_object = &data[free_head];
    if (--free_count == 0) {
      free_head = count;
      free_count = 1;
    } else {
      free_head = new_object->pool_id;
    }
    Memory::call_constructor<T>(new_object);
    new_object->pool_id = (new_object - data) | (pools_id << 16);
    return new_object;
  }

  //---
  inline void free(u32 id)
  {
    --count;
    ++free_count;
    T* object = &data[id & 0xFFFF];
    Memory::call_destructor<T>(object);
    object->pool_id = free_head | 0xFFFF0000;
    free_head = (object - data);
  }

  //---
  inline void free(T* object)
  {
    free(object->pool_id);
  }

  //---
  inline void clear()
  {
    count = 0;
    free_head = 0;
    free_count = 1;
  }

  //---
  template<typename Callable>
  inline bool iterate(Callable&& function)
  {
    u32 objects_visited = 0;
    u32 original_count = count;
    for (u32 m = 0; m < CAPACITY; ++m) {
      if (objects_visited++ >= original_count) {
        break;
      }
      T* pool_object = &data[m];
      if (!is_occupied(pool_object->pool_id)) {
        continue;
      }
      if (!function(pool_object)) {
        return false;
      }
    }
    return true;
  }
};
}