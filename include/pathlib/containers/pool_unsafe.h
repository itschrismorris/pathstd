/*
  Documentation: https://www.path.blog/docs/pool.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/errors/errors.h"
#include "pathlib/memory/memory.h"
#include "pathlib/memory/memset_unsafe.h"
#include "pathlib/string/fixed_string_unsafe.h"

//---
CHECK_HAS_MEMBER(has_pool_id, _pool_id);

namespace Pathlib {

//---
template <typename T, 
          u32 CAPACITY>
struct PoolUnsafe
{
  //---
  static_assert(CAPACITY <= Types::U16_MAX, "Pool CAPACITY cannot exceed 65535 (16-bits used for _'pool_id').");
  static_assert(has_pool_id<T>::value, "Pool objects must contain a u32 member named '_pool_id' to be used in a pool.");
  using POOL_ID_TYPE = _member_type<T, decltype(&T::_pool_id)>::value;
  static_assert(SAME_TYPE(POOL_ID_TYPE, u32), "Pool object member '_pool_id' must be of type u32.");

  //---
  static constexpr u64 EMPTY_SLOT = 0xFFFF0000;

  //---
  T* _data;
  u32 _count;
  u32 _free_count;
  u32 _free_head;
  u32 _pools_id;

  //---
  PoolUnsafe(const utf8* name,
             u32 pools_id = 0)
  {
    _count = 0;
    _free_count = 1;
    _free_head = 0;
    _pools_id = pools_id;
    _data = (T*)malloc_unsafe(sizeof(T) * CAPACITY, 
                              name ? FixedStringUnsafe<64>(u8"\"", name, u8"\"::_data")._str : nullptr);
    memset_unsafe(_data, 0xFF, sizeof(T) * CAPACITY);
  }

  //---
  DISALLOW_COPY(PoolUnsafe);

  //---
  ~PoolUnsafe()
  {
    iterate([&](T& object)
      {
        Memory::call_destructor<T>(&object);
        return true;
      });
    if (_data) {
      free_unsafe((void**)&_data);
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
    if (_count >= CAPACITY) {
      get_errors().to_log_with_stacktrace(u8"Failed to alloc() from pool; it is already at capacity.");
      return nullptr;
    }
    ++_count;
    T* new_object = &_data[_free_head];
    if (--_free_count == 0) {
      _free_head = _count;
      _free_count = 1;
    } else {
      _free_head = new_object->_pool_id;
    }
    Memory::call_constructor<T>(new_object, constructor_args...);
    new_object->_pool_id = (new_object - _data) | (_pools_id << 16);
    return new_object;
  }

  //---
  void free(u32 id)
  {
    --_count;
    ++_free_count;
    T* object = &_data[id & 0xFFFF];
    Memory::call_destructor<T>(object);
    object->_pool_id = _free_head | 0xFFFF0000;
    _free_head = (object - _data);
  }

  //---
  void free(T& object)
  {
    free(object._pool_id);
  }

  //---
  void clear()
  {
    _count = 0;
    _free_head = 0;
    _free_count = 1;
  }

  //---
  template<typename Callable>
  bool iterate(Callable&& function)
  {
    static_assert(HasTParameter<T&, Callable>::value, 
                  "Pool iteration callback must take a parameter with a reference to the pool object type: '(T& object)'");
    static_assert(SAME_TYPE(result_of<Callable(T&)>::value, bool), 
                  "Pool iteration callback must return a bool for continuing or breaking from the iteration.");
    u32 objects_visited = 0;
    u32 original_count = _count;
    for (u32 m = 0; m < CAPACITY; ++m) {
      if (objects_visited++ >= original_count) {
        break;
      }
      T& pool_object = _data[m];
      if (!is_occupied(pool_object._pool_id)) {
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