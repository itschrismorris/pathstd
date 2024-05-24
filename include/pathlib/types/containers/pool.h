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
  static_assert(CAPACITY <= Types::U16_MAX, "Pool CAPACITY cannot exceed 65535 (16-bits used for ID).");

  //---
  static constexpr u32 POOLS_ID_SHIFT = 16;
  static constexpr u32 POOL_ID_MASK = Types::U16_MAX;
  static constexpr u32 FREE_SLOT = 0xFFFF0000;
  static constexpr u32 POOL_ID_INVALID = Types::U32_MAX;

  //---
  u16 pools_id;
  u16 count;
  u16 free_count;
  u16 free_head;
  T* data;
  
  //---
  Pool(u32 _pools_id = 0) 
  {
    pools_id = _pools_id;
    count = 0;
    free_count = 1;
    free_head = 0;
    data = (T*)MALLOC(sizeof(T) * CAPACITY);
  }

  //---
  ~Pool() 
  {
    if (count > 0) {
      error.last_error.format(count, u8" pool member(s) did not free() before pool destruction.");
      error.to_log();
    }
    if (data) {
      FREE(data);
    }
  }

  //---
  inline T* alloc()
  {
    if (count >= CAPACITY) {
      error.last_error.format(u8"Failed to alloc() from pool; it is at capacity.");
      error.to_log();
      return nullptr;
    }
    ++count;
    T* new_item = &data[free_head];
    if (--free_count == 0) {
      free_head = count;
      free_count = 1;
    } else {
      free_head = new_item->pool_id;
    }
    Memory::call_constructor<T>(new_item);
    new_item->pool_id = (new_item - data) | (pools_id << POOLS_ID_SHIFT);
    return new_item;
  }

  //---
  inline void free(T* item)
  {
    if (item) {
      --count;
      ++free_count;
      Memory::call_destructor<T>(item);
      item->pool_id = free_head | 0xFFFF0000;
      free_head = (item - data);
    } else {
      error.last_error.format(u8"Attempted to free a nullptr from pool.");
      error.to_log();
    }
  }


  /*
  inline void initiate(const char* name = nullptr)
  {
    this->capacity = XMMin(1U << POOL_ID_SHIFT, MATH::next_power_of_two(COUNT));
    count = 0;
    free_head = (POOL_FREE << POOL_ID_SHIFT);
    free_count = 1;
    if (name) {
      this->name = name;
      data = (T*)ALIGNED_MALLOC(sizeof(T) * capacity, 32, this->name.str);
    } else {
      data = (T*)ALIGNED_MALLOC(sizeof(T) * capacity, 32);
    }
    memset(data, 0xFF, sizeof(T) * capacity);
  }
  
  inline void shutdown()
  {
    ALIGNED_FREE(data, 32);
  }
 
  inline T* get_vacant(u32 pool_id = 0)
  {
    if (count >= capacity) {
      return nullptr;
    }
    ++count;
    T* new_item = &data[free_head & POOL_ID_MASK];
    if (--free_count == 0) {
      free_head = count | (POOL_FREE << POOL_ID_SHIFT);
      free_count = 1;
    } else {
      free_head = new_item->pool_id;
    }
    new_item->pool_id = get_index(new_item) | (pool_id << POOL_ID_SHIFT);
    return new_item;
  }

  template<typename Callable>
  inline bool iterate(Callable function)
  {
    u32 objects_visited = 0;
    u32 original_count = count;
    for (u32 m = 0; m < capacity; ++m) {
      if (objects_visited >= original_count) {
        break;
      }
      T* pool_item = &data[m];
      if (!is_occupied(pool_item->pool_id)) {
        continue;
      }
      ++objects_visited;
      if (!function(pool_item)) {
        return false;
      }
    }
    return true;
  }

  template<typename Callable>
  inline bool iterate_bool(Callable function)
  {
    u32 objects_visited = 0;
    for (u32 m = 0; m < capacity; ++m) {
      if (objects_visited >= count) {
        break;
      }
      T* pool_item = &data[m];
      if (!is_occupied(pool_item->pool_id)) {
        continue;
      }
      ++objects_visited;
      if (!function(pool_item)) {
        return false;
      }
    }
    return true;
  }

  inline u32 get_index(T* item)
  {
    return (item - data);
  }

  static inline u32 get_index(u32 id)
  {
    return (id & POOL_ID_MASK);
  }

  static inline u32 get_pool_id(u32 id)
  {
    return (id >> POOL_ID_SHIFT);
  }

  static inline bool is_occupied(u32 id) 
  {
    return ((id >> POOL_ID_SHIFT) < POOL_FREE);
  }

  inline void free(T* item)
  {
    --count;
    ++free_count;
    item->pool_id = free_head;
    free_head = (item - data) | (POOL_FREE << POOL_ID_SHIFT);
  }
  
  inline void free(u32 id)
  {
    --count;
    ++free_count;
    u32 index = get_index(id);
    T* item = &data[index];
    item->pool_id = free_head;
    free_head = index | (POOL_FREE << POOL_ID_SHIFT);
  }
  
  inline void clear()
  {
    count = 0;
    free_head = (POOL_FREE << POOL_ID_SHIFT);
    free_count = 1;
  }

  inline T* operator[](u32 id)
  {
    return data[get_index(id)];
  }*/
};
}