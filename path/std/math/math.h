/* 'std/math.h'

*/

#pragma once
#include "../types.h"
#include "../intrinsics/intrinsics.h"

namespace Path::Std::Math {

  /**/
  template<typename T> static inline T min(T a, T b) { return (a < b) ? a : b; }
  template<typename T> static inline T max(T a, T b) { return (a > b) ? a : b; }
  template<typename T> constexpr bool is_multiple(T value, T multiple) { return ((value % multiple) == 0); }
  template<typename T> constexpr bool is_power_of_two(T value) { return ((value & (value - 1)) == 0); }
  static inline u32 first_bit_set(u32 mask) { return _tzcnt_u32(mask); }
  static inline u64 first_bit_set(u64 mask) { return _tzcnt_u64(mask); }
  template<typename T> static inline T* align_previous_32(T* ptr) { return (T*)((u64)ptr & (~31)); };
  template<typename T> static inline T* align_next_32(T* ptr) { return (T*)(((u64)ptr + 31) & (~31)); };
  template<typename T> static inline T* align_previous_64(T* ptr) { return (T*)((u64)ptr & (~63)); };
  template<typename T> static inline T* align_next_64(T* ptr) { return (T*)(((u64)ptr + 63) & (~63)); };
  template<typename T> static inline T* align_previous_128(T* ptr) { return (T*)((u64)ptr & (~127)); };
  template<typename T> static inline T* align_next_128(T* ptr) { return (T*)(((u64)ptr + 127) & (~127)); };
  template<typename T> static inline bool is_aligned_32(T* ptr) { return (((u64)ptr & 31) == 0); }
}

/**/
#define BLEND_AAAA 0x0
#define BLEND_BAAA 0x1
#define BLEND_ABAA 0x2
#define BLEND_BBAA 0x3
#define BLEND_AABA 0x4
#define BLEND_BABA 0x5
#define BLEND_ABBA 0x6
#define BLEND_BBBA 0x7
#define BLEND_AAAB 0x8
#define BLEND_BAAB 0x9
#define BLEND_ABAB 0xA
#define BLEND_BBAB 0xB
#define BLEND_AABB 0xC
#define BLEND_BABB 0xD
#define BLEND_ABBB 0xE
#define BLEND_BBBB 0xF

/**/
#define SHUFF_XXXX _MM_SHUFFLE(0, 0, 0, 0)
#define SHUFF_YYYY _MM_SHUFFLE(1, 1, 1, 1)
#define SHUFF_ZZZZ _MM_SHUFFLE(2, 2, 2, 2)
#define SHUFF_WWWW _MM_SHUFFLE(3, 3, 3, 3)
#define SHUFF_XXYY _MM_SHUFFLE(1, 1, 0, 0)
#define SHUFF_ZZYY _MM_SHUFFLE(1, 1, 2, 2)
#define SHUFF_XXZZ _MM_SHUFFLE(2, 2, 0, 0)
#define SHUFF_YYZZ _MM_SHUFFLE(2, 2, 1, 1)
#define SHUFF_YZXW _MM_SHUFFLE(3, 0, 2, 1)
#define SHUFF_ZWXY _MM_SHUFFLE(1, 0, 3, 2)
#define SHUFF_ZXYW _MM_SHUFFLE(3, 1, 0, 2)
#define SHUFF_YYWW _MM_SHUFFLE(3, 3, 1, 1)
#define SHUFF_YXWZ _MM_SHUFFLE(2, 3, 0, 1)
#define SHUFF_ZZWW _MM_SHUFFLE(3, 3, 2, 2)
#define SHUFF_XYXY _MM_SHUFFLE(1, 0, 1, 0)
#define SHUFF_XZXZ _MM_SHUFFLE(2, 0, 2, 0)
#define SHUFF_ZYZY _MM_SHUFFLE(1, 2, 1, 2)
#define SHUFF_ZYXY _MM_SHUFFLE(1, 0, 1, 2)
#define SHUFF_XZXY _MM_SHUFFLE(1, 0, 2, 0)
#define SHUFF_ZYXZ _MM_SHUFFLE(2, 0, 1, 2)
#define SHUFF_YZWX _MM_SHUFFLE(0, 3, 2, 1)
#define SHUFF_XZYW _MM_SHUFFLE(3, 1, 2, 0)
#define SHUFF_ZYXW _MM_SHUFFLE(3, 0, 1, 2)
#define SHUFF_YWYW _MM_SHUFFLE(3, 1, 3, 1)
#define SHUFF_ZWZW _MM_SHUFFLE(3, 2, 3, 2)
#define SHUFF_WWXX _MM_SHUFFLE(0, 0, 3, 3)
#define SHUFF_XWXW _MM_SHUFFLE(3, 0, 3, 0)
#define SHUFF_WXWX _MM_SHUFFLE(0, 3, 0, 3)
#define SHUFF_WYWY _MM_SHUFFLE(1, 3, 1, 3)
#define SHUFF_ZXZX _MM_SHUFFLE(0, 2, 0, 2)
#define SHUFF_XXWW _MM_SHUFFLE(3, 3, 0, 0)

/**/
#define PREFETCH_L1(A) _mm_prefetch((const char*)A, _MM_HINT_T0)
#define PREFETCH_L2(A) _mm_prefetch((const char*)A, _MM_HINT_T1)
#define PREFETCH_L3(A) _mm_prefetch((const char*)A, _MM_HINT_T2)
#define PREFETCH_NONTEMPORAL(A) _mm_prefetch((const char*)A, _MM_HINT_NTA)
#define FENCE() _mm_sfence()

/**/
#define F4_LOAD(A) _mm_load_ps((F4*)(A))
#define F4_LOADU(A) _mm_loadu_ps((F4*)(A))
#define F4_STORE(A, B) _mm_store_ps(A, B)
#define F4_STOREU(A, B) _mm_storeu_ps(A, B)
#define F4_BROADCAST(A) _mm_broadcast_ss(A)
#define F4_CVT_I4(A) _mm_cvtps_epi32(A)
#define F4_CAST_I4(A) _mm_castps_si128(A)
#define F4_MOVEMASK(A) _mm_movemask_ps(A)
#define F4_SETZERO() _mm_setzero_ps()
#define F4_SET1(A) _mm_set1_ps(A)
#define F4_SET(A, B, C, D) _mm_setr_ps(A, B, C, D)
#define F4_BLEND(A, B, M) _mm_blend_ps(A, B, M)
#define F4_SHUFFLE(A, B, M) _mm_shuffle_ps(A, B, M)
#define F4_PERM(A, M) _mm_castsi128_ps(\
                      _mm_shuffle_epi32(_mm_castps_si128(A), M))
#define F4_BLEND(A, B, M) _mm_blend_ps(A, B, M)
#define F4_TRANSPOSE(A, B, C, D) _MM_TRANSPOSE4_PS(A, B, C, D);
#define F4_ROUND(A, M) _mm_round_ps(A, M)
#define F4_NEG(A) _mm_xor_ps(_mm_set1_ps(-0.0f), A)
#define F4_SUB(A, B) _mm_sub_ps(A, B)
#define F4_ADD(A, B) _mm_add_ps(A, B)
#define F4_HADD(A, B) _mm_hadd_ps(A, B)
#define F4_MUL(A, B) _mm_mul_ps(A, B)
#define F4_FMA(A, B, C) _mm_fmadd_ps(A, B, C)
#define F4_DIV(A, B) _mm_div_ps(A, B)
#define F4_SQRT(A) _mm_sqrt_ps(A)
#define F4_RCP(A) _mm_rcp_ps(A)
#define F4_LENGTH(A) _mm_sqrt_ps(_mm_dp_ps(A, A, 0x7F))
#define F4_NORM(A) _mm_div_ps(A, _mm_sqrt_ps(_mm_dp_ps(A, A, 0x7F)))
#define F4_DP(A, B) _mm_dp_ps(A, B, 0x7F) 
#define F4_MIN(A, B) _mm_min_ps(A, B)
#define F4_MAX(A, B) _mm_max_ps(A, B)
#define F4_HMIN(A) F4_MIN(F4_MIN(A, F4_PERM(A, SHUFF_YZXW)), \
                          F4_PERM(A, SHUFF_ZXYW))
#define F4_HMAX(A) F4_MAX(F4_MAX(A, F4_PERM(A, SHUFF_YZXW)), \
                          F4_PERM(A, SHUFF_ZXYW))
#define F4_XOR(A, B) _mm_xor_ps(A, B)
#define F4_ANDNOT(A, B) _mm_andnot_ps(A, B)
#define F4_AND(A, B) _mm_and_ps(A, B)
#define F4_OR(A, B) _mm_or_ps(A, B)
#define F4_ABS(A) F4_ANDNOT(F4_SET1(-0.0f), A)
#define F4_CMP_EQ(A, B) _mm_cmpeq_ps(A, B)
#define F4_CMP_GT(A, B) _mm_cmpgt_ps(A, B)
#define F4_CMP_GE(A, B) _mm_cmpge_ps(A, B)
#define F4_CMP_LT(A, B) _mm_cmplt_ps(A, B)

/**/
#define I4_LOAD(A) _mm_load_si128((I4*)(A))
#define I4_LOADU(A) _mm_loadu_si128((I4*)(A))
#define I4_STORE(A, B) _mm_store_si128(A, B)
#define I4_STOREU(A, B) _mm_storeu_si128(A, B)
#define I4_STORE_NONTEMPORAL(A, B) _mm_stream_si128(A, B)
#define I4_MOVEMASK(A) _mm_movemask_epi8(A)
#define I4_SETZERO() _mm_setzero_si128()
#define I4_SET(A, B, C, D) _mm_setr_epi32(A, B, C, D)
#define I16_SET _mm_setr_epi8
#define I4_SET1(A) _mm_set1_epi32(A)
#define I4_SETZERO() _mm_setzero_si128()
#define I4_EXTRACT(A) _mm_extract_epi32(A, 0x0)
#define I4_CVT_F4(A) _mm_cvtepi32_ps(A)
#define I4_SHUFFLE8(A, B) _mm_shuffle_epi8(A, B)
#define I4_PERM(A, P) _mm_shuffle_epi32(A, P)
#define I4_PERM32(A, P) _mm_permutevar8x32_epi32(A, P)
#define I4_BLEND8(A, B, C) _mm_blendv_epi8(A, B, C)
#define I4_SUB(A, B) _mm_sub_epi32(A, B)
#define I4_ADD(A, B) _mm_add_epi32(A, B)
#define I4_MUL(A, B) _mm_mullo_epi32(A, B)
#define I4_MIN(A, B) _mm_min_epi32(A, B)
#define I4_MAX(A, B) _mm_max_epi32(A, B)
#define I4_AND(A, B) _mm_and_si128(A, B)
#define I4_ANDNOT(A, B) _mm_andnot_si128(A, B)
#define I4_OR(A, B) _mm_or_si128(A, B)
#define I4_XOR(A, B) _mm_xor_si128(A, B)
#define I4_CMP_GT(A, B) _mm_cmpgt_epi32(A, B)
#define I4_TESTZ(A, B) _mm_testz_si128(A, B)
#define I4_SHIFTL(A, B) _mm_slli_epi32(A, B)
#define I4_SHIFTR(A, B) _mm_srli_epi32(A, B)

/**/
#define I8_LOAD(A) _mm256_load_si256((I8*)(A))
#define I8_LOADU(A) _mm256_loadu_si256((I8*)(A))
#define I8_STORE(A, B) _mm256_store_si256(A, B)
#define I8_STOREU(A, B) _mm256_storeu_si256(A, B)
#define I8_STORE_NONTEMPORAL(A, B) _mm256_stream_si256(A, B)
#define I8_MOVEMASK(A) _mm256_movemask_epi8(A)
#define I8_SETZERO() _mm256_setzero_si256()
#define I8_SET(A, B, C, D, E, F, G, H) _mm256_setr_epi32(A, B, C, D, E, F, G, H)
#define I8_SET1(A) _mm256_set1_epi32(A)
#define I8_SHIFTR(A, B) _mm256_srli_epi32(A, B)
#define I8_MUL(A, B) _mm256_mullo_epi32(A, B)
#define I8_XOR(A, B) _mm256_xor_si256(A, B)
#define I8_CMP_EQ8(A, B) _mm256_cmpeq_epi8(A, B)