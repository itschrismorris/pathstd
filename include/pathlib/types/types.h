/*
  Documentation: https://www.path.blog/docs/types.html
*/

#pragma once

//---
typedef unsigned __int8   u8;
typedef   signed __int8   i8;
typedef unsigned __int16 u16;
typedef   signed __int16 i16;
typedef unsigned __int32 u32;
typedef   signed __int32 i32;
typedef unsigned __int64 u64;
typedef   signed __int64 i64;
typedef float  f32;
typedef double f64;
typedef char8_t utf8;

namespace Pathlib::Types {

  //---
  static constexpr u16 U16_MAX = 0xFFFFU;
  static constexpr u32 U32_MAX = 0xFFFFFFFFU;
  static constexpr u64 U64_MAX = 0xFFFFFFFFFFFFFFFFLLU;
  static constexpr i16 I16_MAX = 0x7FFF;
  static constexpr i32 I32_MAX = 0x7FFFFFFF;
  static constexpr i64 I64_MAX = 0x7FFFFFFFFFFFFFFFLL;
}

//---
template <typename T> struct vec2 { T x, y; };
template <typename T> struct vec3 { T x, y, z; };
template <typename T> struct alignas(16) vec4 { T x, y, z, w; };
template <typename T> struct alignas(32) vec8 { vec4<T> lo; vec4<T> hi; };

//---
#define I4 __m128i
#define I8 __m256i
#define F4 __m128
#define F8 __m256

//---
static constexpr u32 CACHE_LINE_SIZE = 64;

//---
#define EXPORT __declspec(dllexport)
#define SAME_TYPE(A, B) __is_same(A, B)
#define IS_VEC2(A) _is_vec2<A>::value
#define IS_VEC3(A) _is_vec3<A>::value
#define IS_VEC4(A) _is_vec4<A>::value
#define IS_VEC8(A) _is_vec8<A>::value
#define IS_INTEGRAL(A) _is_integral<A>::value
#define IS_FLOAT(A) _is_float<A>::value
#define IS_SHORT_STRING(A) _is_short_string<A>::value
#define IS_LONG_STRING(A) _is_long_string<A>::value
#define IS_POINTER(A) _is_pointer<A>::value
#define EXPECT(A) __builtin_expect((A), 1)
#define DISALLOW_COPY(T) T(const T&) = delete;         \
                         T& operator=(const T&) = delete;

//---
struct false_type { static constexpr bool value = false; constexpr operator bool() const { return value; } };
struct true_type { static constexpr bool value = true; constexpr operator bool() const { return value; } };

//---
template <typename...>
using void_t = void;

//---
template <typename T>
T declval() noexcept {}

//---
template <typename>
struct result_of;
template <typename F, 
          typename... Args>
struct result_of<F(Args...)> 
{
  using type = decltype(declval<F>()(declval<Args>()...));
};

//---
template <typename T, 
          typename... Args>
struct has_constructor 
{
  template <typename U, typename... UArgs>
  static auto test(U* u) -> decltype(new U(declval<UArgs>()...), true_type{});
  template <typename, typename...>
  static false_type test(...);
  static constexpr bool value = decltype(test<T, Args...>(nullptr))::value;
};

//---
template <typename T>
struct has_destructor {
  template <typename U>
  static auto test(U* u) -> decltype(u->~U(), true_type{});
  template <typename>
  static false_type test(...);
  static constexpr bool value = decltype(test<T>(nullptr))::value;
};

//---
#define CHECK_HAS_MEMBER(CALL_NAME, MEMBER_NAME) \
  template <typename T, typename = void> \
  struct CALL_NAME : false_type {}; \
  template <typename T> \
  struct CALL_NAME<T, void_t<decltype(declval<T>().MEMBER_NAME)>> : true_type {};

//---
template <typename T, typename U> struct _member_type;
template <typename T, typename U> struct _member_type<T, U T::*> { using type = U; };

//---
template <typename T> struct _is_pointer { static constexpr bool value = false; };
template <typename T> struct _is_pointer<T*> { static constexpr bool value = true; };
template <typename T> struct _is_integral : false_type {};
template <> struct _is_integral<i8> : true_type {};
template <> struct _is_integral<i16> : true_type {};
template <> struct _is_integral<i32> : true_type {};
template <> struct _is_integral<i64> : true_type {};
template <> struct _is_integral<u8> : true_type {};
template <> struct _is_integral<u16> : true_type {};
template <> struct _is_integral<u32> : true_type {};
template <> struct _is_integral<u64> : true_type {};
template <typename T> struct _is_float : false_type {};
template <> struct _is_float<f32> : true_type {};
template <> struct _is_float<f64> : true_type {};
template <typename T> struct _is_vec2 : false_type {};
template <typename T> struct _is_vec2<vec2<T>> : true_type {};
template <typename T> struct _is_vec3 : false_type {};
template <typename T> struct _is_vec3<vec3<T>> : true_type {};
template <typename T> struct _is_vec4 : false_type {};
template <typename T> struct _is_vec4<vec4<T>> : true_type {};
template <typename T> struct _is_vec8 : false_type {};
template <typename T> struct _is_vec8<vec8<T>> : true_type {};

//---
template <typename T> struct return_false : false_type {};