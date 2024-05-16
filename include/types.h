/*
  Documentation: https://www.path.blog/docs/types.html
*/

#pragma once

#define EXPORT __declspec(dllexport)
#define U64_MAX 0xFFFFFFFFFFFFFFFFLLU
#define U32_MAX 0xFFFFFFFFU
#define I64_MAX 0x7FFFFFFFFFFFFFFFLL
#define I32_MAX 0x7FFFFFFF
#define SAME_TYPE(A, B) __is_same(A, B)
#define IS_VEC2(A) _is_vec2<A>::value
#define IS_VEC3(A) _is_vec3<A>::value
#define IS_VEC4(A) _is_vec4<A>::value
#define IS_VEC8(A) _is_vec8<A>::value
#define IS_INTEGRAL(A) _is_integral<A>::value
#define IS_FLOAT(A) _is_float<A>::value
#define IS_LOCAL_STRING(A) _is_local_string<A>::value

/**/
struct false_type { static constexpr bool value = false; constexpr operator bool() const { return value; } };
struct true_type { static constexpr bool value = true; constexpr operator bool() const { return value; } };

/**/
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

/**/
template <typename T> struct vec2 { T x, y; };
template <typename T> struct vec3 { T x, y, z; };
template <typename T> struct alignas(16) vec4 { T x, y, z, w; };
template <typename T> struct alignas(32) vec8 { vec4<T> lo; vec4<T> hi; };

/**/
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
template <> struct _is_integral<f32> : true_type {};
template <> struct _is_integral<f64> : true_type {};
template <typename T> struct _is_vec2 : false_type {};
template <typename T> struct _is_vec2<vec2<T>> : true_type {};
template <typename T> struct _is_vec3 : false_type {};
template <typename T> struct _is_vec3<vec3<T>> : true_type {};
template <typename T> struct _is_vec4 : false_type {};
template <typename T> struct _is_vec4<vec4<T>> : true_type {};
template <typename T> struct _is_vec8 : false_type {};
template <typename T> struct _is_vec8<vec8<T>> : true_type {};

#define I4 __m128i
#define I8 __m256i
#define F4 __m128
#define F8 __m256