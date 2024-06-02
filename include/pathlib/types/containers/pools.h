/*
  Documentation: https://www.path.blog/docs/pools.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"
#include "pathlib/types/containers/pool_unsafe.h"

namespace Pathlib {

/**/
template <typename T, 
          u32 POOL_CAPACITY,
          u32 POOLS_RESERVE_CAPACITY>
struct Pools
{
  //---
  static_assert(POOL_CAPACITY <= Types::U16_MAX, "POOL_CAPACITY cannot exceed 65535 (16-bits used for pool_id).");
  static_assert(has_pool_id<T>::value, "Pool objects must contain a u32 member named 'pool_id' to be used in a pool.");
  using POOL_ID_TYPE = _member_type<T, decltype(&T::pool_id)>::type;
  static_assert(SAME_TYPE(POOL_ID_TYPE, u32), "Pool object member 'pool_id' must be of type u32.");

private:
  //---
  VectorUnsafe<PoolUnsafe<T, POOL_CAPACITY>, POOLS_RESERVE_CAPACITY> pools;
  u64 count;
  
public:
  //---
  Pools() 
  {
    pools.emplace_back(1);
    count = 0;
  }
  ~Pools() {}

  //---
  static inline bool is_occupied(u32 id)
  {
    return (id < 0xFFFF0000);
  }

  //---
  inline SafePtr<T> get_vacant()
  {
    for (u32 p = 0; p < pools.count; ++p) {
      if (pools[p].count < pools[p].capacity()) {
        ++count;
        return pools[p].get_vacant(p);
      }
    }
    pools.emplace_back(1);
    ++count;
    return SafePtr<T>(pools[pools.count - 1].get_vacant(pools.count - 1));
  }
  
  //---
  inline void free(u32 id)
  {
    pools[id >> 16].free(id);
    --count;
  }

  //---
  inline void free(T& object)
  {
    free(object.pool_id);
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
    for (u64 p = 0; p < pools.count; ++p) {
      if (!pools[p].iterate(function)) {
        return false;
      }
    }
    return true;
  }

  //---
  inline void clear()
  {
    for (u32 p = 0; p < pools.count; ++p) {
      pools[p].clear();
    }
    count = 0;
  }
};
}