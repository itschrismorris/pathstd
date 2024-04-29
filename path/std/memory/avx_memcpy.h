/* 'std/string/avx_memcpy.h'

  + Utilizes SSE, AVX, non-temporal prefetch, and stores.
  + Alignment hints in template optimize instructions used.
  + Small copies (256 bytes or less) use optimized jump-table.
  + Large copies use non-temporal prefetch and stores.

    Path game engine: https://www.path.blog
*/

#pragma once
#include "std/memory/memory.h"
#include "std/math/math.h"

namespace Path::Std::String {

/**/
template <u64 REGISTER, u64 REGISTER_COUNT, bool DST_ALIGNED = false, bool SRC_ALIGNED = false>
static inline void sse_memcpy_registers(void* dst,
                                        const void* src)
{
  I4 m[REGISTER_COUNT];
  m[REGISTER] = SRC_ALIGNED ? I4_LOAD((I4*)src + REGISTER) : I4_LOADU((I4*)src + REGISTER);
  DST_ALIGNED ? I4_STORE((I4*)dst + REGISTER, m[REGISTER]) : I4_STOREU((I4*)dst + REGISTER, m[REGISTER]);
  if constexpr((REGISTER + 1) < REGISTER_COUNT) {
    sse_memcpy_registers<REGISTER + 1, REGISTER_COUNT, DST_ALIGNED, SRC_ALIGNED>(dst, src);
  }
}

/**/
template <u64 REGISTER, u64 REGISTER_COUNT, bool DST_ALIGNED = false, bool SRC_ALIGNED = false>
static inline void avx_memcpy_registers(void* dst,
                                        const void* src)
{
  I8 m[REGISTER_COUNT];
  m[REGISTER] = SRC_ALIGNED ? I8_LOAD((I8*)src + REGISTER) : I8_LOADU((I8*)src + REGISTER);
  DST_ALIGNED ? I8_STORE((I8*)dst + REGISTER, m[REGISTER]) : I8_STOREU((I8*)dst + REGISTER, m[REGISTER]);
  if constexpr ((REGISTER + 1) < REGISTER_COUNT) {
    avx_memcpy_registers<REGISTER + 1, REGISTER_COUNT, DST_ALIGNED, SRC_ALIGNED>(dst, src);
  }
}

/**/
template <bool DST_ALIGNED = false, bool SRC_ALIGNED = false>
static inline void *avx_memcpy_256(void* dst, 
                                   const void* src, 
                                   u64 size)
{
  /*
  u32 avx_register_count = size / 128;
  u32 sse_register_count = (size % 32) / 16;
  u32 extra_byte_count = (size % 16);
  
  switch (avx_register_count) {
    case 0: break;
    case 1: avx_memcpy_registers<0, 1, DST_ALIGNED, SRC_ALIGNED>(dst, src);
    case 2: avx_memcpy_registers<0, 2, DST_ALIGNED, SRC_ALIGNED>(dst, src);
    case 3: avx_memcpy_registers<0, 3, DST_ALIGNED, SRC_ALIGNED>(dst, src);
    case 4: avx_memcpy_registers<0, 4, DST_ALIGNED, SRC_ALIGNED>(dst, src);
    case 5: avx_memcpy_registers<0, 5, DST_ALIGNED, SRC_ALIGNED>(dst, src);
    case 6: avx_memcpy_registers<0, 6, DST_ALIGNED, SRC_ALIGNED>(dst, src);
    case 7: avx_memcpy_registers<0, 7, DST_ALIGNED, SRC_ALIGNED>(dst, src);
    case 8: avx_memcpy_registers<0, 8, DST_ALIGNED, SRC_ALIGNED>(dst, src);
  }
  if (sse_register_count) {
    sse_memcpy_registers<0, 1, DST_ALIGNED, SRC_ALIGNED>(dst, src);
  }
  u8* _dst = (u8*)dst + (avx_register_count * 32) + (sse_register_count * 16);
  u8* _src = (u8*)src + (avx_register_count * 32) + (sse_register_count * 16);
  for (u32 b = 0; b < extra_byte_count; ++b) {
    _dst[b] = _src[b];
  }*/

  u8* _dst = ((u8*)dst) + size;
  const u8* _src = ((const u8*)src) + size;
  switch (size) {
    case 128: avx_memcpy_registers<0, 4, false, false>(_dst - 128, _src - 128);
    case 0:  break;
    case 129: avx_memcpy_registers<0, 4, false, false>(_dst - 129, _src - 129);
    case 1: _dst[-1] = _src[-1]; break;
    case 130: avx_memcpy_registers<0, 4, false, false>(_dst - 130, _src - 130);
    case 2: *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 131: avx_memcpy_registers<0, 4, false, false>(_dst - 131, _src - 131);
    case 3: *((u16*)(_dst - 3)) = *((u16*)(_src - 3)); _dst[-1] = _src[-1]; break;
    case 132: avx_memcpy_registers<0, 4, false, false>(_dst - 132, _src - 132);
    case 4: *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 133: avx_memcpy_registers<0, 4, false, false>(_dst - 133, _src - 133);
    case 5: *((u32*)(_dst - 5)) = *((u32*)(_src - 5)); _dst[-1] = _src[-1]; break;
    case 134: avx_memcpy_registers<0, 4, false, false>(_dst - 134, _src - 134);
    case 6: *((u32*)(_dst - 6)) = *((u32*)(_src - 6)); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 135: avx_memcpy_registers<0, 4, false, false>(_dst - 135, _src - 135);
    case 7: *((u32*)(_dst - 7)) = *((u32*)(_src - 7)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 136: avx_memcpy_registers<0, 4, false, false>(_dst - 136, _src - 136);
    case 8: *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 137: avx_memcpy_registers<0, 4, false, false>(_dst - 137, _src - 137);
    case 9: *((u64*)(_dst - 9)) = *((u64*)(_src - 9)); _dst[-1] = _src[-1]; break;
    case 138: avx_memcpy_registers<0, 4, false, false>(_dst - 138, _src - 138);
    case 10: *((u64*)(_dst - 10)) = *((u64*)(_src - 10)); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 139: avx_memcpy_registers<0, 4, false, false>(_dst - 139, _src - 139);
    case 11: *((u64*)(_dst - 11)) = *((u64*)(_src - 11)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 140: avx_memcpy_registers<0, 4, false, false>(_dst - 140, _src - 140);
    case 12: *((u64*)(_dst - 12)) = *((u64*)(_src - 12)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 141: avx_memcpy_registers<0, 4, false, false>(_dst - 141, _src - 141);
    case 13: *((u64*)(_dst - 13)) = *((u64*)(_src - 13)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 142: avx_memcpy_registers<0, 4, false, false>(_dst - 142, _src - 142);
    case 14: *((u64*)(_dst - 14)) = *((u64*)(_src - 14)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 143: avx_memcpy_registers<0, 4, false, false>(_dst - 143, _src - 143);
    case 15: *((u64*)(_dst - 15)) = *((u64*)(_src - 15)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 144: avx_memcpy_registers<0, 4, false, false>(_dst - 144, _src - 144);
    case 16: sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 145: avx_memcpy_registers<0, 4, false, false>(_dst - 145, _src - 145);
    case 17: sse_memcpy_registers<0, 1, false, false>(_dst - 17, _src - 17); _dst[-1] = _src[-1]; break;
    case 146: avx_memcpy_registers<0, 4, false, false>(_dst - 146, _src - 146);
    case 18: sse_memcpy_registers<0, 1, false, false>(_dst - 18, _src - 18); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 147: avx_memcpy_registers<0, 4, false, false>(_dst - 147, _src - 147);
    case 19: sse_memcpy_registers<0, 1, false, false>(_dst - 19, _src - 19); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 148: avx_memcpy_registers<0, 4, false, false>(_dst - 148, _src - 148);
    case 20: sse_memcpy_registers<0, 1, false, false>(_dst - 20, _src - 20); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 149: avx_memcpy_registers<0, 4, false, false>(_dst - 149, _src - 149);
    case 21: sse_memcpy_registers<0, 1, false, false>(_dst - 21, _src - 21); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 150: avx_memcpy_registers<0, 4, false, false>(_dst - 150, _src - 150);
    case 22: sse_memcpy_registers<0, 1, false, false>(_dst - 22, _src - 22); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 151: avx_memcpy_registers<0, 4, false, false>(_dst - 151, _src - 151);
    case 23: sse_memcpy_registers<0, 1, false, false>(_dst - 23, _src - 23); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 152: avx_memcpy_registers<0, 4, false, false>(_dst - 152, _src - 152);
    case 24: sse_memcpy_registers<0, 1, false, false>(_dst - 24, _src - 24); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 153: avx_memcpy_registers<0, 4, false, false>(_dst - 153, _src - 153);
    case 25: sse_memcpy_registers<0, 1, false, false>(_dst - 25, _src - 25); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 154: avx_memcpy_registers<0, 4, false, false>(_dst - 154, _src - 154);
    case 26: sse_memcpy_registers<0, 1, false, false>(_dst - 26, _src - 26); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 155: avx_memcpy_registers<0, 4, false, false>(_dst - 155, _src - 155);
    case 27: sse_memcpy_registers<0, 1, false, false>(_dst - 27, _src - 27); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 156: avx_memcpy_registers<0, 4, false, false>(_dst - 156, _src - 156);
    case 28: sse_memcpy_registers<0, 1, false, false>(_dst - 28, _src - 28); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 157: avx_memcpy_registers<0, 4, false, false>(_dst - 157, _src - 157);
    case 29: sse_memcpy_registers<0, 1, false, false>(_dst - 29, _src - 29); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 158: avx_memcpy_registers<0, 4, false, false>(_dst - 158, _src - 158);
    case 30: sse_memcpy_registers<0, 1, false, false>(_dst - 30, _src - 30); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 159: avx_memcpy_registers<0, 4, false, false>(_dst - 159, _src - 159);
    case 31: sse_memcpy_registers<0, 1, false, false>(_dst - 31, _src - 31); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 160: avx_memcpy_registers<0, 4, false, false>(_dst - 160, _src - 160);
    case 32: avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 161: avx_memcpy_registers<0, 4, false, false>(_dst - 161, _src - 161);
    case 33: avx_memcpy_registers<0, 1, false, false>(_dst - 33, _src - 33); _dst[-1] = _src[-1]; break;
    case 162: avx_memcpy_registers<0, 4, false, false>(_dst - 162, _src - 162);
    case 34: avx_memcpy_registers<0, 1, false, false>(_dst - 34, _src - 34); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 163: avx_memcpy_registers<0, 4, false, false>(_dst - 163, _src - 163);
    case 35: avx_memcpy_registers<0, 1, false, false>(_dst - 35, _src - 35); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 164: avx_memcpy_registers<0, 4, false, false>(_dst - 164, _src - 164);
    case 36: avx_memcpy_registers<0, 1, false, false>(_dst - 36, _src - 36); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 165: avx_memcpy_registers<0, 4, false, false>(_dst - 165, _src - 165);
    case 37: avx_memcpy_registers<0, 1, false, false>(_dst - 37, _src - 37); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 166: avx_memcpy_registers<0, 4, false, false>(_dst - 166, _src - 166);
    case 38: avx_memcpy_registers<0, 1, false, false>(_dst - 38, _src - 38); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 167: avx_memcpy_registers<0, 4, false, false>(_dst - 167, _src - 167);
    case 39: avx_memcpy_registers<0, 1, false, false>(_dst - 39, _src - 39); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 168: avx_memcpy_registers<0, 4, false, false>(_dst - 168, _src - 168);
    case 40: avx_memcpy_registers<0, 1, false, false>(_dst - 40, _src - 40); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 169: avx_memcpy_registers<0, 4, false, false>(_dst - 169, _src - 169);
    case 41: avx_memcpy_registers<0, 1, false, false>(_dst - 41, _src - 41); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 170: avx_memcpy_registers<0, 4, false, false>(_dst - 170, _src - 170);
    case 42: avx_memcpy_registers<0, 1, false, false>(_dst - 42, _src - 42); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 171: avx_memcpy_registers<0, 4, false, false>(_dst - 171, _src - 171);
    case 43: avx_memcpy_registers<0, 1, false, false>(_dst - 43, _src - 43); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 172: avx_memcpy_registers<0, 4, false, false>(_dst - 172, _src - 172);
    case 44: avx_memcpy_registers<0, 1, false, false>(_dst - 44, _src - 44); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 173: avx_memcpy_registers<0, 4, false, false>(_dst - 173, _src - 173);
    case 45: avx_memcpy_registers<0, 1, false, false>(_dst - 45, _src - 45); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 174: avx_memcpy_registers<0, 4, false, false>(_dst - 174, _src - 174);
    case 46: avx_memcpy_registers<0, 1, false, false>(_dst - 46, _src - 46); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 175: avx_memcpy_registers<0, 4, false, false>(_dst - 175, _src - 175);
    case 47: avx_memcpy_registers<0, 1, false, false>(_dst - 47, _src - 47); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 176: avx_memcpy_registers<0, 4, false, false>(_dst - 176, _src - 176);
    case 48: avx_memcpy_registers<0, 1, false, false>(_dst - 48, _src - 48); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 177: avx_memcpy_registers<0, 4, false, false>(_dst - 177, _src - 177);
    case 49: avx_memcpy_registers<0, 1, false, false>(_dst - 49, _src - 49); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 178: avx_memcpy_registers<0, 4, false, false>(_dst - 178, _src - 178);
    case 50: avx_memcpy_registers<0, 1, false, false>(_dst - 50, _src - 50); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 179: avx_memcpy_registers<0, 4, false, false>(_dst - 179, _src - 179);
    case 51: avx_memcpy_registers<0, 1, false, false>(_dst - 51, _src - 51); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 180: avx_memcpy_registers<0, 4, false, false>(_dst - 180, _src - 180);
    case 52: avx_memcpy_registers<0, 1, false, false>(_dst - 52, _src - 52); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 181: avx_memcpy_registers<0, 4, false, false>(_dst - 181, _src - 181);
    case 53: avx_memcpy_registers<0, 1, false, false>(_dst - 53, _src - 53); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 182: avx_memcpy_registers<0, 4, false, false>(_dst - 182, _src - 182);
    case 54: avx_memcpy_registers<0, 1, false, false>(_dst - 54, _src - 54); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 183: avx_memcpy_registers<0, 4, false, false>(_dst - 183, _src - 183);
    case 55: avx_memcpy_registers<0, 1, false, false>(_dst - 55, _src - 55); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 184: avx_memcpy_registers<0, 4, false, false>(_dst - 184, _src - 184);
    case 56: avx_memcpy_registers<0, 1, false, false>(_dst - 56, _src - 56); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 185: avx_memcpy_registers<0, 4, false, false>(_dst - 185, _src - 185);
    case 57: avx_memcpy_registers<0, 1, false, false>(_dst - 57, _src - 57); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 186: avx_memcpy_registers<0, 4, false, false>(_dst - 186, _src - 186);
    case 58: avx_memcpy_registers<0, 1, false, false>(_dst - 58, _src - 58); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 187: avx_memcpy_registers<0, 4, false, false>(_dst - 187, _src - 187);
    case 59: avx_memcpy_registers<0, 1, false, false>(_dst - 59, _src - 59); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 188: avx_memcpy_registers<0, 4, false, false>(_dst - 188, _src - 188);
    case 60: avx_memcpy_registers<0, 1, false, false>(_dst - 60, _src - 60); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 189: avx_memcpy_registers<0, 4, false, false>(_dst - 189, _src - 189);
    case 61: avx_memcpy_registers<0, 1, false, false>(_dst - 61, _src - 61); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 190: avx_memcpy_registers<0, 4, false, false>(_dst - 190, _src - 190);
    case 62: avx_memcpy_registers<0, 1, false, false>(_dst - 62, _src - 62); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 191: avx_memcpy_registers<0, 4, false, false>(_dst - 191, _src - 191);
    case 63: avx_memcpy_registers<0, 1, false, false>(_dst - 63, _src - 63); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 192: avx_memcpy_registers<0, 4, false, false>(_dst - 192, _src - 192);
    case 64: avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 193: avx_memcpy_registers<0, 4, false, false>(_dst - 193, _src - 193);
    case 65: avx_memcpy_registers<0, 2, false, false>(_dst - 65, _src - 65); _dst[-1] = _src[-1]; break;
    case 194: avx_memcpy_registers<0, 4, false, false>(_dst - 194, _src - 194);
    case 66: avx_memcpy_registers<0, 2, false, false>(_dst - 66, _src - 66); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 195: avx_memcpy_registers<0, 4, false, false>(_dst - 195, _src - 195);
    case 67: avx_memcpy_registers<0, 2, false, false>(_dst - 67, _src - 67); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 196: avx_memcpy_registers<0, 4, false, false>(_dst - 196, _src - 196);
    case 68: avx_memcpy_registers<0, 2, false, false>(_dst - 68, _src - 68); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 197: avx_memcpy_registers<0, 4, false, false>(_dst - 197, _src - 197);
    case 69: avx_memcpy_registers<0, 2, false, false>(_dst - 69, _src - 69); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 198: avx_memcpy_registers<0, 4, false, false>(_dst - 198, _src - 198);
    case 70: avx_memcpy_registers<0, 2, false, false>(_dst - 70, _src - 70); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 199: avx_memcpy_registers<0, 4, false, false>(_dst - 199, _src - 199);
    case 71: avx_memcpy_registers<0, 2, false, false>(_dst - 71, _src - 71); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 200: avx_memcpy_registers<0, 4, false, false>(_dst - 200, _src - 200);
    case 72: avx_memcpy_registers<0, 2, false, false>(_dst - 72, _src - 72); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 201: avx_memcpy_registers<0, 4, false, false>(_dst - 201, _src - 201);
    case 73: avx_memcpy_registers<0, 2, false, false>(_dst - 73, _src - 73); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 202: avx_memcpy_registers<0, 4, false, false>(_dst - 202, _src - 202);
    case 74: avx_memcpy_registers<0, 2, false, false>(_dst - 74, _src - 74); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 203: avx_memcpy_registers<0, 4, false, false>(_dst - 203, _src - 203);
    case 75: avx_memcpy_registers<0, 2, false, false>(_dst - 75, _src - 75); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 204: avx_memcpy_registers<0, 4, false, false>(_dst - 204, _src - 204);
    case 76: avx_memcpy_registers<0, 2, false, false>(_dst - 76, _src - 76); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 205: avx_memcpy_registers<0, 4, false, false>(_dst - 205, _src - 205);
    case 77: avx_memcpy_registers<0, 2, false, false>(_dst - 77, _src - 77); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 206: avx_memcpy_registers<0, 4, false, false>(_dst - 206, _src - 206);
    case 78: avx_memcpy_registers<0, 2, false, false>(_dst - 78, _src - 78); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 207: avx_memcpy_registers<0, 4, false, false>(_dst - 207, _src - 207);
    case 79: avx_memcpy_registers<0, 2, false, false>(_dst - 79, _src - 79); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 208: avx_memcpy_registers<0, 4, false, false>(_dst - 208, _src - 208);
    case 80: avx_memcpy_registers<0, 2, false, false>(_dst - 80, _src - 80); sse_memcpy_registers<0, 1, false, false>(_dst - 16, _src - 16); break;
    case 209: avx_memcpy_registers<0, 4, false, false>(_dst - 209, _src - 209);
    case 81: avx_memcpy_registers<0, 2, false, false>(_dst - 81, _src - 81); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 210: avx_memcpy_registers<0, 4, false, false>(_dst - 210, _src - 210);
    case 82: avx_memcpy_registers<0, 2, false, false>(_dst - 82, _src - 82); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 211: avx_memcpy_registers<0, 4, false, false>(_dst - 211, _src - 211);
    case 83: avx_memcpy_registers<0, 2, false, false>(_dst - 83, _src - 83); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 212: avx_memcpy_registers<0, 4, false, false>(_dst - 212, _src - 212);
    case 84: avx_memcpy_registers<0, 2, false, false>(_dst - 84, _src - 84); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 213: avx_memcpy_registers<0, 4, false, false>(_dst - 213, _src - 213);
    case 85: avx_memcpy_registers<0, 2, false, false>(_dst - 85, _src - 85); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 214: avx_memcpy_registers<0, 4, false, false>(_dst - 214, _src - 214);
    case 86: avx_memcpy_registers<0, 2, false, false>(_dst - 86, _src - 86); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 215: avx_memcpy_registers<0, 4, false, false>(_dst - 215, _src - 215);
    case 87: avx_memcpy_registers<0, 2, false, false>(_dst - 87, _src - 87); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 216: avx_memcpy_registers<0, 4, false, false>(_dst - 216, _src - 216);
    case 88: avx_memcpy_registers<0, 2, false, false>(_dst - 88, _src - 88); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 217: avx_memcpy_registers<0, 4, false, false>(_dst - 217, _src - 217);
    case 89: avx_memcpy_registers<0, 2, false, false>(_dst - 89, _src - 89); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 218: avx_memcpy_registers<0, 4, false, false>(_dst - 218, _src - 218);
    case 90: avx_memcpy_registers<0, 2, false, false>(_dst - 90, _src - 90); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 219: avx_memcpy_registers<0, 4, false, false>(_dst - 219, _src - 219);
    case 91: avx_memcpy_registers<0, 2, false, false>(_dst - 91, _src - 91); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 220: avx_memcpy_registers<0, 4, false, false>(_dst - 220, _src - 220);
    case 92: avx_memcpy_registers<0, 2, false, false>(_dst - 92, _src - 92); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 221: avx_memcpy_registers<0, 4, false, false>(_dst - 221, _src - 221);
    case 93: avx_memcpy_registers<0, 2, false, false>(_dst - 93, _src - 93); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 222: avx_memcpy_registers<0, 4, false, false>(_dst - 222, _src - 222);
    case 94: avx_memcpy_registers<0, 2, false, false>(_dst - 94, _src - 94); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 223: avx_memcpy_registers<0, 4, false, false>(_dst - 223, _src - 223);
    case 95: avx_memcpy_registers<0, 2, false, false>(_dst - 95, _src - 95); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 224: avx_memcpy_registers<0, 4, false, false>(_dst - 224, _src - 224);
    case 96: avx_memcpy_registers<0, 2, false, false>(_dst - 96, _src - 96); avx_memcpy_registers<0, 1, false, false>(_dst - 32, _src - 32); break;
    case 225: avx_memcpy_registers<0, 4, false, false>(_dst - 225, _src - 225);
    case 97: avx_memcpy_registers<0, 2, false, false>(_dst - 97, _src - 97); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 226: avx_memcpy_registers<0, 4, false, false>(_dst - 226, _src - 226);
    case 98: avx_memcpy_registers<0, 2, false, false>(_dst - 98, _src - 98); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 227: avx_memcpy_registers<0, 4, false, false>(_dst - 227, _src - 227);
    case 99: avx_memcpy_registers<0, 2, false, false>(_dst - 99, _src - 99); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 228: avx_memcpy_registers<0, 4, false, false>(_dst - 228, _src - 228);
    case 100: avx_memcpy_registers<0, 2, false, false>(_dst - 100, _src - 100); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 229: avx_memcpy_registers<0, 4, false, false>(_dst - 229, _src - 229);
    case 101: avx_memcpy_registers<0, 2, false, false>(_dst - 101, _src - 101); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 230: avx_memcpy_registers<0, 4, false, false>(_dst - 230, _src - 230);
    case 102: avx_memcpy_registers<0, 2, false, false>(_dst - 102, _src - 102); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 231: avx_memcpy_registers<0, 4, false, false>(_dst - 231, _src - 231);
    case 103: avx_memcpy_registers<0, 2, false, false>(_dst - 103, _src - 103); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 232: avx_memcpy_registers<0, 4, false, false>(_dst - 232, _src - 232);
    case 104: avx_memcpy_registers<0, 2, false, false>(_dst - 104, _src - 104); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 233: avx_memcpy_registers<0, 4, false, false>(_dst - 233, _src - 233);
    case 105: avx_memcpy_registers<0, 2, false, false>(_dst - 105, _src - 105); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 234: avx_memcpy_registers<0, 4, false, false>(_dst - 234, _src - 234);
    case 106: avx_memcpy_registers<0, 2, false, false>(_dst - 106, _src - 106); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 235: avx_memcpy_registers<0, 4, false, false>(_dst - 235, _src - 235);
    case 107: avx_memcpy_registers<0, 2, false, false>(_dst - 107, _src - 107); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 236: avx_memcpy_registers<0, 4, false, false>(_dst - 236, _src - 236);
    case 108: avx_memcpy_registers<0, 2, false, false>(_dst - 108, _src - 108); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 237: avx_memcpy_registers<0, 4, false, false>(_dst - 237, _src - 237);
    case 109: avx_memcpy_registers<0, 2, false, false>(_dst - 109, _src - 109); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 238: avx_memcpy_registers<0, 4, false, false>(_dst - 238, _src - 238);
    case 110: avx_memcpy_registers<0, 2, false, false>(_dst - 110, _src - 110); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 239: avx_memcpy_registers<0, 4, false, false>(_dst - 239, _src - 239);
    case 111: avx_memcpy_registers<0, 2, false, false>(_dst - 111, _src - 111); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 240: avx_memcpy_registers<0, 4, false, false>(_dst - 240, _src - 240);
    case 112: avx_memcpy_registers<0, 2, false, false>(_dst - 112, _src - 112); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 241: avx_memcpy_registers<0, 4, false, false>(_dst - 241, _src - 241);
    case 113: avx_memcpy_registers<0, 2, false, false>(_dst - 113, _src - 113); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 242: avx_memcpy_registers<0, 4, false, false>(_dst - 242, _src - 242);
    case 114: avx_memcpy_registers<0, 2, false, false>(_dst - 114, _src - 114); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 243: avx_memcpy_registers<0, 4, false, false>(_dst - 243, _src - 243);
    case 115: avx_memcpy_registers<0, 2, false, false>(_dst - 115, _src - 115); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 244: avx_memcpy_registers<0, 4, false, false>(_dst - 244, _src - 244);
    case 116: avx_memcpy_registers<0, 2, false, false>(_dst - 116, _src - 116); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 245: avx_memcpy_registers<0, 4, false, false>(_dst - 245, _src - 245);
    case 117: avx_memcpy_registers<0, 2, false, false>(_dst - 117, _src - 117); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 246: avx_memcpy_registers<0, 4, false, false>(_dst - 246, _src - 246);
    case 118: avx_memcpy_registers<0, 2, false, false>(_dst - 118, _src - 118); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 247: avx_memcpy_registers<0, 4, false, false>(_dst - 247, _src - 247);
    case 119: avx_memcpy_registers<0, 2, false, false>(_dst - 119, _src - 119); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 248: avx_memcpy_registers<0, 4, false, false>(_dst - 248, _src - 248);
    case 120: avx_memcpy_registers<0, 2, false, false>(_dst - 120, _src - 120); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 249: avx_memcpy_registers<0, 4, false, false>(_dst - 249, _src - 249);
    case 121: avx_memcpy_registers<0, 2, false, false>(_dst - 121, _src - 121); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 250: avx_memcpy_registers<0, 4, false, false>(_dst - 250, _src - 250);
    case 122: avx_memcpy_registers<0, 2, false, false>(_dst - 122, _src - 122); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 251: avx_memcpy_registers<0, 4, false, false>(_dst - 251, _src - 251);
    case 123: avx_memcpy_registers<0, 2, false, false>(_dst - 123, _src - 123); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 252: avx_memcpy_registers<0, 4, false, false>(_dst - 252, _src - 252);
    case 124: avx_memcpy_registers<0, 2, false, false>(_dst - 124, _src - 124); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 253: avx_memcpy_registers<0, 4, false, false>(_dst - 253, _src - 253);
    case 125: avx_memcpy_registers<0, 2, false, false>(_dst - 125, _src - 125); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 254: avx_memcpy_registers<0, 4, false, false>(_dst - 254, _src - 254);
    case 126: avx_memcpy_registers<0, 2, false, false>(_dst - 126, _src - 126); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 255: avx_memcpy_registers<0, 4, false, false>(_dst - 255, _src - 255);
    case 127: avx_memcpy_registers<0, 2, false, false>(_dst - 127, _src - 127); avx_memcpy_registers<0, 2, false, false>(_dst - 64, _src - 64); break;
    case 256: avx_memcpy_registers<0, 8, false, false>(_dst - 256, _src - 256); break;
  }

  _mm256_zeroupper();
  return dst;
}

/**/
template <bool DST_ALIGNED = false, bool SRC_ALIGNED = false>
static void* avx_memcpy(void* dst, 
                        const void* src, 
                        u64 size)
{
	u8* _dst = (u8*)dst;
  const u8* _src = (const u8*)src;
	if (size <= 256) {
    avx_memcpy_256<DST_ALIGNED, SRC_ALIGNED>(_dst, _src, size);
		return dst;
	}
  I8 c[8];
  if (DST_ALIGNED) {
    PREFETCH_NONTEMPORAL(_src);
    for (; size >= 256; size -= 256) {
      #pragma clang loop unroll(full)
      for (u32 r = 0; r < 8; ++r) {
        c[r] = SRC_ALIGNED ? I8_LOAD((I8*)_src + r) : I8_LOADU((I8*)_src + r);
      }
      PREFETCH_NONTEMPORAL(_src + 512);
      _src += 256;
      #pragma clang loop unroll(full)
      for (u32 r = 0; r < 8; ++r) {
        c[r] = I8_LOAD((I8*)_src + r);
        I8_STORE_NONTEMPORAL((((I8*)_dst) + r), c[r]);
      }
      _dst += 256;
    }
  } else {
    u64 padding = (32 - ((u64)_dst & 31)) & 31;
	  I8 head = _mm256_loadu_si256((const I8*)_src);
	  _mm256_storeu_si256((I8*)_dst, head);
    _dst += padding;
    _src += padding;
	  size -= padding;
    PREFETCH_NONTEMPORAL(_src);
	  if (Math::is_aligned<32>(_src)) {
		  for (; size >= 256; size -= 256) {
        #pragma clang loop unroll(full)
        for (u32 r = 0; r < 8; ++r) {
          c[r] = I8_LOAD((I8*)_src + r);
        }
        PREFETCH_NONTEMPORAL(src + 512);
        _src += 256;
        #pragma clang loop unroll(full)
        for (u32 r = 0; r < 8; ++r) {
          c[r] = I8_LOAD((I8*)_src + r);
          I8_STORE_NONTEMPORAL((((I8*)_dst) + r), c[r]);
        }
        _dst += 256;
		  }
	  } else {
		  for (; size >= 256; size -= 256) {
        #pragma clang loop unroll(full)
        for (u32 r = 0; r < 8; ++r) {
          c[r] = I8_LOADU((I8*)_src + r);
        }
        PREFETCH_NONTEMPORAL(_src + 512);
        _src += 256;
        #pragma clang loop unroll(full)
        for (u32 r = 0; r < 8; ++r) {
          c[r] = I8_LOAD((I8*)_src + r);
          I8_STORE_NONTEMPORAL((((I8*)_dst) + r), c[r]);
        }
        _dst += 256;
		  }
	  }
  }
  FENCE();
  avx_memcpy_256<DST_ALIGNED, SRC_ALIGNED>(_dst, _src, size);
  return dst;
}
}