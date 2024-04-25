/* 'pathstd/string/avx_strlen.h'

  + Utilizes AVX SIMD to read length of string.
  + Protected against bad page boundary crossing.
  + To check 128 bytes per iteration, we first read previous 128-byte aligned address and mask out unused bits.
  + Then we can continue to loop over aligned 128-byte reads without paging issues.
*/

#pragma once
#include <algorithm>
#include <stdint.h>
#include <immintrin.h>

/**/
#define GET_CMP_MASK(S) ((uint32_t)_mm256_movemask_epi8( \
                                     _mm256_cmpeq_epi8( \
                                       _mm256_load_si256((__m256i*)(S)), zero)))

/**/
static inline size_t avx_strlen(const char* s)
{
  __m256i zero = _mm256_setzero_si256();
  uint64_t counted = 0;
  const char* _s = (const char*)((uintptr_t)s & (~127));
  int32_t ignore_bytes = s - _s;
  if (ignore_bytes < 64) {
    uint64_t check32 = GET_CMP_MASK(_s);
    uint64_t check64 = GET_CMP_MASK(_s + 32);
    uint64_t zero_mask64 = (check32 | (check64 << 32)) & 
                           (UINT64_MAX << ignore_bytes);
    if (zero_mask64) {
      return _tzcnt_u64(zero_mask64) - ignore_bytes;
    }
  }
  uint64_t check96 = GET_CMP_MASK(_s + 64);
  uint64_t check128 = GET_CMP_MASK(_s + 96);
  uint64_t zero_mask128 = (check96 | (check128 << 32)) & 
                          (UINT64_MAX << std::max(0, ignore_bytes - 64));
  if (zero_mask128) {
    return (64 - ignore_bytes + _tzcnt_u64(zero_mask128));
  }
  counted += 128 - ignore_bytes;
  s = _s + 128;
  while (true) {
    uint64_t check32 = GET_CMP_MASK(s);
    uint64_t check64 = GET_CMP_MASK(s + 32);
    uint64_t zero_mask64 = (check32 | (check64 << 32));
    if (zero_mask64) {
      return (counted + _tzcnt_u64(zero_mask64));
    }
    uint64_t check96 = GET_CMP_MASK(s + 64);
    uint64_t check128 = GET_CMP_MASK(s + 96);
    uint64_t zero_mask128 = (check96 | (check128 << 32));
    if (zero_mask128) {
      return (counted + 64 + _tzcnt_u64(zero_mask128));
    }
    counted += 128;
    s += 128;
  }
  return 0;
}