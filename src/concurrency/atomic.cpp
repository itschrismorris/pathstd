#include <pathlib/concurrency/atomic.h>

namespace Pathlib {

//---
template <typename T>
Atomic<T>::Atomic() {}

//---
template <typename T>
Atomic<T>::Atomic(const T value) : _value(value) {}

//---
template <typename T>
Atomic<T>::~Atomic() {}

//---
template <typename T>
T Atomic<T>::operator =(const T value)
{
  store(value);
  return value;
}

//---
template <typename T>
void Atomic<T>::store(const T value,
                      i32 mem_order)
{
  __atomic_store_n(&_value, value, mem_order);
}

//---
template <typename T>
inline T Atomic<T>::load(i32 mem_order)
{
  return __atomic_load_n(&_value, mem_order);
}

//---
template <typename T>
inline T Atomic<T>::swap(const T value,
                         i32 mem_order)
{
  return __atomic_exchange_n(&_value, value, mem_order);
}

//---
template <typename T>
inline bool Atomic<T>::compare_and_swap(const T expected_value,
                                        const T value_if_expected,
                                        i32 mem_order)
{
  return __atomic_compare_exchange_n(&_value, (T*)&expected_value, value_if_expected,
    true, mem_order, mem_order);
}

//---
template <typename T>
inline T Atomic<T>::fetch_add(const T arg,
                              i32 mem_order)
{
  return __atomic_add_fetch(&_value, arg, mem_order);
}

//---
template <typename T>
inline T Atomic<T>::fetch_sub(const T arg,
                              i32 mem_order)
{
  return __atomic_sub_fetch(&_value, arg, mem_order);
}

//--
template <typename T>
inline T Atomic<T>::fetch_and(const T arg,
                              i32 mem_order)
{
  return __atomic_and_fetch(&_value, arg, mem_order);
}

//--
template <typename T>
inline T Atomic<T>::fetch_xor(const T arg,
                              i32 mem_order)
{
  return __atomic_xor_fetch(&_value, arg, mem_order);
}

//--
template <typename T>
inline T Atomic<T>::fetch_or(const T arg,
                             i32 mem_order)
{
  return __atomic_or_fetch(&_value, arg, mem_order);
}

//--
template <typename T>
inline T Atomic<T>::fetch_not_and(const T arg,
                                  i32 mem_order)
{
  return __atomic_nand_fetch(&_value, arg, mem_order);
}

//---
AtomicFlag::AtomicFlag() {}

//---
AtomicFlag::~AtomicFlag() {}

//---
AtomicFlag::AtomicFlag(const u8 value)
{
  if (_value == 0) {
    clear();
  } else {
    test_and_set();
  }
}

//---
bool AtomicFlag::test(i32 mem_order) const
{
  return __atomic_load_n(&_value, mem_order);
}

//---
inline bool AtomicFlag::test_and_set(i32 mem_order)
{
  return __atomic_test_and_set(&_value, mem_order);
}

//---
inline void AtomicFlag::clear(i32 mem_order)
{
  __atomic_clear(&_value, mem_order);
}

//---
INSTANTIATE_FOR_INTEGERS(Atomic);
}
