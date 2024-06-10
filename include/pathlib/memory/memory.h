/*
  Documentation: https://www.path.blog/docs/memory.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/arena.h"

//---
inline void* operator new(size_t, void* ptr)
{
  return ptr;
}

namespace Pathlib {
  
//---
struct MemoryName 
{
  //---
  const utf8* _name;

  //---
  explicit MemoryName(const char8_t* name) { _name = name ? name : u8"Unnamed"; }
  ~MemoryName() {}

  //---
  inline constexpr const utf8* get() const { return _name; }

  //---
  inline constexpr const utf8* operator()() const { return _name; }
};

namespace _Internal {

struct Memory
{
  //---
  static constexpr u32 PAGE_SIZE = 0x1000;
  static constexpr u32 KILOBYTE = 1024;
  static constexpr u32 MEGABYTE = (1024 * 1024);
  static constexpr u32 GIGABYTE = (1024 * 1024 * 1024);

  //---
  Arena persistent_arena;
  Arena scoped_arena;

  //---
  Memory();
  ~Memory();

  //---
  bool enable_large_pages();
};
}

//---
template <typename T,
          typename... Args>
static inline void call_constructor(void* obj,
                                    Args&&... args)
{
  if constexpr (has_constructor<T, Args...>::value) {
    new (obj) T(args...);
  }
}

//---
template <typename T>
static inline void call_destructor(T* obj)
{
  if constexpr (has_destructor<T>::value) {
    obj->~T();
  }
}
}

//---
namespace Pathlib { _Internal::Memory& get_memory(); }