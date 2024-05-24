/*
  Documentation: https://www.path.blog/docs/pools.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/error/error.h"

namespace Pathlib::Containers {

/**/
template <typename T, 
          u16 POOL_CAPACITY,
          u32 POOLS_RESERVE_CAPACITY>
struct Pools
{
  //---
  Vector<Pool<T, POOL_CAPACITY>, POOLS_RESERVE_CAPACITY> pools;
  u32 count;
  
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
  inline T* alloc()
  {
    for (u32 p = 0; p < pools.count; ++p) {
      if (pools[p].count < pools[p].capacity()) {
        ++count;
        return pools[p].alloc(p);
      }
    }
    pools.emplace_back(1);
    ++count;
    return pools[pools.count - 1].alloc(pools.count - 1);
  }
  
  //---
  inline void free(u32 id)
  {
    pools[id >> 16].free(id);
    --count;
  }

  //---
  inline void free(T* object)
  {
    free(object->pool_id);
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