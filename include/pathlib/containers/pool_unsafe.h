/*
  Documentation: https://www.path.blog/docs/pool.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/errors/errors.h"
#include "pathlib/memory/memory.h"
#include "pathlib/memory/memset.h"
#include "pathlib/string/short_string_unsafe.h"

//---
CHECK_HAS_MEMBER(has_pool_id, pool_id);

namespace Pathlib {

//---
template <typename T, 
          u32 CAPACITY>
struct PoolUnsafe
{
  //---
  static_assert(CAPACITY <= Types::U16_MAX, "Pool CAPACITY cannot exceed 65535 (16-bits used for pool_id).");
  static_assert(has_pool_id<T>::value, "Pool objects must contain a u32 member named 'pool_id' to be used in a pool.");
  using POOL_ID_TYPE = _member_type<T, decltype(&T::pool_id)>::type;
  static_assert(SAME_TYPE(POOL_ID_TYPE, u32), "Pool object member 'pool_id' must be of type u32.");

  //---
  static constexpr u64 EMPTY_SLOT = 0xFFFF0000;

  //---
  T* data;
  u32 count;
  u32 free_count;
  u32 free_head;
  u32 pools_id;
  
  //---
  PoolUnsafe(const utf8* name,
             u32 _pools_id = 0)
  {
    count = 0;
    free_count = 1;
    free_head = 0;
    pools_id = _pools_id;
    data = (T*)malloc_unsafe(sizeof(T) * CAPACITY, 
                             name ? ShortStringUnsafe<96>(u8"[Pool]\"", name, u8"\"::[T*]data").str : nullptr);
    memset_unsafe(data, 0xFF, sizeof(T) * CAPACITY);
  }

  //---
  ~PoolUnsafe()
  {
    iterate([&](T& object)
      {
        Memory::call_destructor<T>(&object);
        return true;
      });
    if (data) {
      free_unsafe((void**)&data);
    }
  }

  //---
  bool is_occupied(u32 id)
  {
    return (((id & 0xFFFF) < CAPACITY) && (id < 0xFFFF0000));
  }

  //---
  template <typename... Args>
  T* get_vacant(Args&&... constructor_args)
  {
    if (count >= CAPACITY) {
      get_errors().set_last_error(u8"Failed to alloc() from pool; it is already at capacity.");
      get_errors().to_log();
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
    Memory::call_constructor<T>(new_object, constructor_args...);
    new_object->pool_id = (new_object - data) | (pools_id << 16);
    return new_object;
  }

  //---
  void free(u32 id)
  {
    --count;
    ++free_count;
    T* object = &data[id & 0xFFFF];
    Memory::call_destructor<T>(object);
    object->pool_id = free_head | 0xFFFF0000;
    free_head = (object - data);
  }

  //---
  void free(T& object)
  {
    free(object.pool_id);
  }

  //---
  void clear()
  {
    count = 0;
    free_head = 0;
    free_count = 1;
  }

  //---
  template<typename Callable>
  bool iterate(Callable&& function)
  {
    static_assert(SAME_TYPE(result_of<Callable(T&)>::type, bool), 
                  "Pool iteration callback must return a bool for continuing or breaking from the iteration.");
    u32 objects_visited = 0;
    u32 original_count = count;
    for (u32 m = 0; m < CAPACITY; ++m) {
      if (objects_visited++ >= original_count) {
        break;
      }
      T& pool_object = data[m];
      if (!is_occupied(pool_object.pool_id)) {
        continue;
      }
      if (!function(pool_object)) {
        return false;
      }
    }
    return true;
  }

  //---
  static constexpr u64 get_capacity()
  {
    return CAPACITY;
  }
};
}