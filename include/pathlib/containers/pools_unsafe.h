/*
  Documentation: https://www.path.blog/docs/pools.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/containers/pool_unsafe.h"
#include "pathlib/containers/vector_unsafe.h"
#include "pathlib/string/fixed_string_unsafe.h"

namespace Pathlib {

/**/
template <typename T, 
          u32 POOL_CAPACITY,
          u32 POOLS_RESERVE_CAPACITY>
struct PoolsUnsafe
{
  //---
  static_assert(POOL_CAPACITY <= Types::U16_MAX, "POOL_CAPACITY cannot exceed 65535 (16-bits used for pool_id).");
  static_assert(has_pool_id<T>::value, "Pool objects must contain a u32 member named '_pool_id' to be used in a pool.");
  using POOL_ID_TYPE = _member_type<T, decltype(&T::_pool_id)>::value;
  static_assert(SAME_TYPE(POOL_ID_TYPE, u32), "Pool object member 'pool_id' must be of type u32.");

  //---
  VectorUnsafe<PoolUnsafe<T, POOL_CAPACITY>, POOLS_RESERVE_CAPACITY> _pools;
  u32 _count;
  FixedStringUnsafe<64> _name;

  //---
  DISALLOW_COPY(PoolsUnsafe);
  
  //---
  explicit PoolsUnsafe(const MemoryName& name) : _pools(FixedStringUnsafe<64>(u8"\"", name(), u8"\"::_pools")._str)
  {
    _count = 0;
    u32 pools_id = 0;
    _pools.emplace_back(1, FixedStringUnsafe<64>(u8"\"", name(), u8"\"[", _count, u8"]")._str, pools_id);
    _name = name();
  }

  /**/
  inline T* operator[](u32 id)
  {
    if (EXPECT(is_occupied(id))) {
      u32 pools_id = (id >> 16);
      return &_pools[pools_id]._data[id & 0xFFFF];
    } else {
      return nullptr;
    }
  }

  ~PoolsUnsafe() {}
  //---
  static inline bool is_occupied(u32 id)
  {
    return (id < 0xFFFF0000);
  }

  //---
  inline T* get_vacant()
  {
    for (u32 p = 0; p < _pools._count; ++p) {
      if (_pools[p]._count < _pools[p].get_capacity()) {
        ++_count;
        return _pools[p].get_vacant();
      }
    }
    ++_count;
    u32 pools_id = _pools._count - 1;
    _pools.emplace_back(1, (_name._size > 0) ? FixedStringUnsafe<64>(u8"\"", _name, u8"\"[", _count, u8"]")._str : nullptr,
                        pools_id);
    return _pools[_pools._count - 1].get_vacant();
  }
  
  //---
  inline void free(u32 id)
  {
    _pools[id >> 16].free(id);
    --_count;
  }

  //---
  inline void free(T& object)
  {
    free(object.pool_id);
  }

  //---
  inline void clear()
  {
    for (u32 p = 0; p < _pools._count; ++p) {
      _pools[p].clear();
    }
    _count = 0;
  }

  //---
  template<typename Callable>
  inline bool iterate(Callable&& function)
  {
    for (u64 p = 0; p < _pools._count; ++p) {
      if (!_pools[p].iterate(function)) {
        return false;
      }
    }
    return true;
  }
};
}