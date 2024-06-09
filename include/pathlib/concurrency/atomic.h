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
  static_assert((sizeof(T) == 1) || (sizeof(T) == 2) || 
                (sizeof(T) == 4) || (sizeof(T) == 8), 
                "Atomic type must be of size 1, 2, 4, or 8 bytes.");

  //---
  volatile T _value;

  //---
  DISALLOW_COPY(Atomic);
  Atomic();
  explicit Atomic(const T value);
  ~Atomic();

  //---
  T operator =(const T value);
  void store(const T value,
             i32 mem_order = MemOrder::SEQ_CST);
  T load(i32 mem_order = MemOrder::SEQ_CST);
  T swap(const T value,
         i32 mem_order = MemOrder::SEQ_CST);
  bool compare_and_swap(const T expected_value,
                        const T value_if_expected,
                        i32 mem_order = MemOrder::SEQ_CST);
  T fetch_add(const T arg,
              i32 mem_order = MemOrder::SEQ_CST);
  T fetch_sub(const T arg,
              i32 mem_order = MemOrder::SEQ_CST);
  T fetch_and(const T arg,
              i32 mem_order = MemOrder::SEQ_CST);
  T fetch_xor(const T arg,
              i32 mem_order = MemOrder::SEQ_CST);
  T fetch_or(const T arg,
             i32 mem_order = MemOrder::SEQ_CST);
  T fetch_not_and(const T arg,
                  i32 mem_order = MemOrder::SEQ_CST);
};

//---
struct AtomicFlag
{
  //---
  volatile u8 _value;

  //---
  DISALLOW_COPY(AtomicFlag);
  AtomicFlag();
  explicit AtomicFlag(const u8 value);
  ~AtomicFlag();

  //---
  bool test(i32 mem_order = MemOrder::SEQ_CST) const;
  bool test_and_set(i32 mem_order = MemOrder::SEQ_CST);
  void clear(i32 mem_order = MemOrder::SEQ_CST);
};
}