/* 'std/types.h'

*/

#pragma once

#define EXPORT __declspec(dllexport)

/**/
typedef signed __int32 Error;
typedef unsigned __int8   u8;
typedef   signed __int8   i8;
typedef unsigned __int16 u16;
typedef   signed __int16 i16;
typedef unsigned __int32 u32;
typedef   signed __int32 i32;
typedef unsigned __int64 u64;
typedef   signed __int64 i64;
typedef __int16 f16;
typedef float  f32;
typedef double f64;

/**/
struct i32_2 { i32 x, y; };
struct i32_3 { i32 x, y, z; };
struct i32_4 { i32 x, y, z, w; };
struct u32_2 { u32 x, y; };
struct u32_3 { u32 x, y, z; };
struct u32_4 { u32 x, y, z, w; };
struct f32_2 { f32 x, y; };
struct f32_3 { f32 x, y, z; };
struct f32_4 { f32 x, y, z, w; };


#define F4 __m128
#define F8 __m256
#define I4 __m128i
#define I8 __m256i
#define F4_SCALAR alignas(16) f32
#define F8_SCALAR alignas(32) f32
#define I4_SCALAR alignas(16) i32
#define I8_SCALAR alignas(32) i32