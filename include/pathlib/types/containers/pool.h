/*
  Documentation: https://www.path.blog/docs/pool.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"
#include "pathlib/types/containers/pool_unsafe.h"

namespace Pathlib {

//---
template <typename T, 
          u32 CAPACITY>
struct Pool
{
  //---
  static_assert(CAPACITY <= Types::U16_MAX, "Pool CAPACITY cannot exceed 65535 (16-bits used for pool_id).");
  static_assert(has_pool_id<T>::value, "Pool objects must contain a u32 member named 'pool_id' to be used in a pool.");
  using POOL_ID_TYPE = _member_type<T, decltype(&T::pool_id)>::type;
  static_assert(SAME_TYPE(POOL_ID_TYPE, u32), "Pool object member 'pool_id' must be of type u32.");

private:
  //---
  bool named;
  u32 count;
  u32 free_count;
  u32 free_head;
  T* data;

public:
  //---
  Pool(const utf8* name) 
  {
    count = 0;
    free_count = 1;
    free_head = 0;
    data = (T*)malloc_unsafe(sizeof(T) * CAPACITY);
    memset_unsafe(data, 0xFF, sizeof(T) * CAPACITY);
  }

  //---
  ~Pool() 
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
  inline SafePtr<T> get_vacant(u32 pools_id = 0)
  {
    if (count >= CAPACITY) {
      error.set_last_error(u8"Failed to alloc() from pool; it is already at capacity.");
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
    return SafePtr<T>(new_object);
  }

  //---
  inline void free(u32 id)
  {
    if (EXPECT(is_occupied(id) && (count > 0))) {
      --count;
      ++free_count;
      T* object = &data[id & 0xFFFF];
      Memory::call_destructor<T>(object);
      object->pool_id = free_head | 0xFFFF0000;
      free_head = (object - data);
    } else {
      error.set_last_error(u8"Attempt to free an invalid pool_id from Pool.");
      error.to_log();
      error.kill_script();
    }
  }

  //---
  inline void free(SafePtr<T>& object)
  {
    if (EXPECT(object.is_valid())) {
      free(object->pool_id);
      object = nullptr;
    } else {
      error.set_last_error(u8"Attempt to free a null SafePtr from Pool.");
      error.to_log();
      error.kill_script();
    }
  }

  //---
  inline void clear()
  {
    count = 0;
    free_head = 0;
    free_count = 1;
  }

  //---
  inline u64 get_count()
  {
    return count;
  }

  //---
  template<typename Callable>
  inline bool iterate(Callable&& function)
  {
    static_assert(SAME_TYPE(result_of<Callable(T&)>::type, bool), "Pool iteration callback must return a bool for continuing or breaking from the iteration.");
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
};
}