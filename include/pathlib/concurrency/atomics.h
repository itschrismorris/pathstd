/*
  Documentation: https://www.path.blog/docs/thread.html
*/

#pragma once
#include "pathlib/types/types.h"

namespace Pathlib {

//---
namespace MemOrder {

  //---
  static constexpr i32 RELAXED = __ATOMIC_RELAXED;
  static constexpr i32 CONSUME = __ATOMIC_CONSUME;
  static constexpr i32 ACQUIRE = __ATOMIC_ACQUIRE;
  static constexpr i32 RELEASE = __ATOMIC_RELEASE;
  static constexpr i32 ACQ_REL = __ATOMIC_ACQ_REL;
  static constexpr i32 SEQ_CST = __ATOMIC_SEQ_CST;
}

//---
template <typename T = u32>
struct Atomic
{
  static_assert((sizeof(T) == 1) || (sizeof(T) == 2) || (sizeof(T) == 4) || (sizeof(T) == 8), 
                "Atomic type must be of size 1, 2, 4, or 8 bytes.");

  //---
  volatile T _value;

  //---
  Atomic() {}
  explicit Atomic(const T value) : _value(value) {}
  DISALLOW_COPY(Atomic);
  ~Atomic() {}

  //---
  inline T operator =(const T value)
  {
    store(value);
    return value;
  }

  //---
  inline void store(const T value,
                    i32 mem_order = MemOrder::SEQ_CST)
  {
    __atomic_store_n(&_value, value, mem_order);
  }

  //---
  inline T load(i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_load_n(&_value, mem_order);
  }

  //---
  inline T swap(const T value,
                i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_exchange_n(&_value, value, mem_order);
  }

  //---
  inline bool compare_and_swap(const T expected_value,
                               const T value_if_expected,
                               i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_compare_exchange_n(&_value, (T*)&expected_value, value_if_expected,
                                       true, mem_order, mem_order);
  }

  //---
  inline T fetch_add(const T arg,
                     i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_add_fetch(&_value, arg, mem_order);
  }

  //---
  inline T fetch_sub(const T arg,
                     i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_sub_fetch(&_value, arg, mem_order);
  }

  //--
  inline T fetch_and(const T arg,
                     i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_and_fetch(&_value, arg, mem_order);
  }

  //--
  inline T fetch_xor(const T arg,
                     i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_xor_fetch(&_value, arg, mem_order);
  }

  //--
  inline T fetch_or(const T arg,
                    i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_or_fetch(&_value, arg, mem_order);
  }

  //--
  inline T fetch_not_and(const T arg,
                         i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_nand_fetch(&_value, arg, mem_order);
  }
};

//---
struct AtomicFlag : Atomic<u8>
{
  //---
  AtomicFlag() {}
  ~AtomicFlag() {}

  //---
  inline bool test(i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_load_n(&_value, mem_order);
  }

  //---
  inline bool test_and_set(i32 mem_order = MemOrder::SEQ_CST)
  {
    return __atomic_test_and_set(&_value, mem_order);
  }

  //---
  inline void clear(i32 mem_order = MemOrder::SEQ_CST)
  {
    __atomic_clear(&_value, mem_order);
  }
};
}