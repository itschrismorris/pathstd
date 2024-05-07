/* 'memory/avx_memcpy.h'

  + Copies 'size' bytes from 'src' to 'dst'.
  + Uses AVX and an optimized jump table for small sizes.
  + Fast path for AVX-aligned src and dst.

    Path game engine: https://www.path.blog
*/

#pragma once
#include "types.h"
#include "memory/memory.h"
#include "math/math.h"

namespace Pathlib::Memory {

/**/
template <u32 REGISTER_COUNT>
static inline void memcpy_sse(u8* dst, 
                              const u8* src) 
{
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I4 m = I4_LOADU(((const I4*)src) + r);
    I4_STOREU(((I4*)dst) + r, m);
  }
}

/**/
template <u32 REGISTER_COUNT>
static inline void memcpy_avx(u8* dst, 
                              const u8* src) 
{
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I8 m = I8_LOADU(((const I8*)src) + r);
    I8_STOREU(((I8*)dst) + r, m);
  }
}

/**/
static inline void memcpy_small(u8* dst, 
                                const u8* src, 
                                u64 size)
{
  dst += size;
  src += size;
  switch (size) {
    case 128: memcpy_avx<4>(dst - 128, src - 128);
    case 0:  break;
    case 129: memcpy_avx<4>(dst - 129, src - 129);
    case 1: dst[-1] = src[-1]; break;
    case 130: memcpy_avx<4>(dst - 130, src - 130);
    case 2: *((u16*)(dst - 2)) = *((u16*)(src - 2)); break;
    case 131: memcpy_avx<4>(dst - 131, src - 131);
    case 3: *((u16*)(dst - 3)) = *((u16*)(src - 3)); dst[-1] = src[-1]; break;
    case 132: memcpy_avx<4>(dst - 132, src - 132);
    case 4: *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 133: memcpy_avx<4>(dst - 133, src - 133);
    case 5: *((u32*)(dst - 5)) = *((u32*)(src - 5)); dst[-1] = src[-1]; break;
    case 134: memcpy_avx<4>(dst - 134, src - 134);
    case 6: *((u32*)(dst - 6)) = *((u32*)(src - 6)); *((u16*)(dst - 2)) = *((u16*)(src - 2)); break;
    case 135: memcpy_avx<4>(dst - 135, src - 135);
    case 7: *((u32*)(dst - 7)) = *((u32*)(src - 7)); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 136: memcpy_avx<4>(dst - 136, src - 136);
    case 8: *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 137: memcpy_avx<4>(dst - 137, src - 137);
    case 9: *((u64*)(dst - 9)) = *((u64*)(src - 9)); dst[-1] = src[-1]; break;
    case 138: memcpy_avx<4>(dst - 138, src - 138);
    case 10: *((u64*)(dst - 10)) = *((u64*)(src - 10)); *((u16*)(dst - 2)) = *((u16*)(src - 2)); break;
    case 139: memcpy_avx<4>(dst - 139, src - 139);
    case 11: *((u64*)(dst - 11)) = *((u64*)(src - 11)); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 140: memcpy_avx<4>(dst - 140, src - 140);
    case 12: *((u64*)(dst - 12)) = *((u64*)(src - 12)); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 141: memcpy_avx<4>(dst - 141, src - 141);
    case 13: *((u64*)(dst - 13)) = *((u64*)(src - 13)); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 142: memcpy_avx<4>(dst - 142, src - 142);
    case 14: *((u64*)(dst - 14)) = *((u64*)(src - 14)); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 143: memcpy_avx<4>(dst - 143, src - 143);
    case 15: *((u64*)(dst - 15)) = *((u64*)(src - 15)); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 144: memcpy_avx<4>(dst - 144, src - 144);
    case 16: memcpy_sse<1>(dst - 16, src - 16); break;
    case 145: memcpy_avx<4>(dst - 145, src - 145);
    case 17: memcpy_sse<1>(dst - 17, src - 17); dst[-1] = src[-1]; break;
    case 146: memcpy_avx<4>(dst - 146, src - 146);
    case 18: memcpy_sse<1>(dst - 18, src - 18); *((u16*)(dst - 2)) = *((u16*)(src - 2)); break;
    case 147: memcpy_avx<4>(dst - 147, src - 147);
    case 19: memcpy_sse<1>(dst - 19, src - 19); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 148: memcpy_avx<4>(dst - 148, src - 148);
    case 20: memcpy_sse<1>(dst - 20, src - 20); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 149: memcpy_avx<4>(dst - 149, src - 149);
    case 21: memcpy_sse<1>(dst - 21, src - 21); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 150: memcpy_avx<4>(dst - 150, src - 150);
    case 22: memcpy_sse<1>(dst - 22, src - 22); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 151: memcpy_avx<4>(dst - 151, src - 151);
    case 23: memcpy_sse<1>(dst - 23, src - 23); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 152: memcpy_avx<4>(dst - 152, src - 152);
    case 24: memcpy_sse<1>(dst - 24, src - 24); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 153: memcpy_avx<4>(dst - 153, src - 153);
    case 25: memcpy_sse<1>(dst - 25, src - 25); memcpy_sse<1>(dst - 16, src - 16); break;
    case 154: memcpy_avx<4>(dst - 154, src - 154);
    case 26: memcpy_sse<1>(dst - 26, src - 26); memcpy_sse<1>(dst - 16, src - 16); break;
    case 155: memcpy_avx<4>(dst - 155, src - 155);
    case 27: memcpy_sse<1>(dst - 27, src - 27); memcpy_sse<1>(dst - 16, src - 16); break;
    case 156: memcpy_avx<4>(dst - 156, src - 156);
    case 28: memcpy_sse<1>(dst - 28, src - 28); memcpy_sse<1>(dst - 16, src - 16); break;
    case 157: memcpy_avx<4>(dst - 157, src - 157);
    case 29: memcpy_sse<1>(dst - 29, src - 29); memcpy_sse<1>(dst - 16, src - 16); break;
    case 158: memcpy_avx<4>(dst - 158, src - 158);
    case 30: memcpy_sse<1>(dst - 30, src - 30); memcpy_sse<1>(dst - 16, src - 16); break;
    case 159: memcpy_avx<4>(dst - 159, src - 159);
    case 31: memcpy_sse<1>(dst - 31, src - 31); memcpy_sse<1>(dst - 16, src - 16); break;
    case 160: memcpy_avx<4>(dst - 160, src - 160);
    case 32: memcpy_avx<1>(dst - 32, src - 32); break;
    case 161: memcpy_avx<4>(dst - 161, src - 161);
    case 33: memcpy_avx<1>(dst - 33, src - 33); dst[-1] = src[-1]; break;
    case 162: memcpy_avx<4>(dst - 162, src - 162);
    case 34: memcpy_avx<1>(dst - 34, src - 34); *((u16*)(dst - 2)) = *((u16*)(src - 2)); break;
    case 163: memcpy_avx<4>(dst - 163, src - 163);
    case 35: memcpy_avx<1>(dst - 35, src - 35); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 164: memcpy_avx<4>(dst - 164, src - 164);
    case 36: memcpy_avx<1>(dst - 36, src - 36); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 165: memcpy_avx<4>(dst - 165, src - 165);
    case 37: memcpy_avx<1>(dst - 37, src - 37); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 166: memcpy_avx<4>(dst - 166, src - 166);
    case 38: memcpy_avx<1>(dst - 38, src - 38); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 167: memcpy_avx<4>(dst - 167, src - 167);
    case 39: memcpy_avx<1>(dst - 39, src - 39); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 168: memcpy_avx<4>(dst - 168, src - 168);
    case 40: memcpy_avx<1>(dst - 40, src - 40); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 169: memcpy_avx<4>(dst - 169, src - 169);
    case 41: memcpy_avx<1>(dst - 41, src - 41); memcpy_sse<1>(dst - 16, src - 16); break;
    case 170: memcpy_avx<4>(dst - 170, src - 170);
    case 42: memcpy_avx<1>(dst - 42, src - 42); memcpy_sse<1>(dst - 16, src - 16); break;
    case 171: memcpy_avx<4>(dst - 171, src - 171);
    case 43: memcpy_avx<1>(dst - 43, src - 43); memcpy_sse<1>(dst - 16, src - 16); break;
    case 172: memcpy_avx<4>(dst - 172, src - 172);
    case 44: memcpy_avx<1>(dst - 44, src - 44); memcpy_sse<1>(dst - 16, src - 16); break;
    case 173: memcpy_avx<4>(dst - 173, src - 173);
    case 45: memcpy_avx<1>(dst - 45, src - 45); memcpy_sse<1>(dst - 16, src - 16); break;
    case 174: memcpy_avx<4>(dst - 174, src - 174);
    case 46: memcpy_avx<1>(dst - 46, src - 46); memcpy_sse<1>(dst - 16, src - 16); break;
    case 175: memcpy_avx<4>(dst - 175, src - 175);
    case 47: memcpy_avx<1>(dst - 47, src - 47); memcpy_sse<1>(dst - 16, src - 16); break;
    case 176: memcpy_avx<4>(dst - 176, src - 176);
    case 48: memcpy_avx<1>(dst - 48, src - 48); memcpy_sse<1>(dst - 16, src - 16); break;
    case 177: memcpy_avx<4>(dst - 177, src - 177);
    case 49: memcpy_avx<1>(dst - 49, src - 49); memcpy_avx<1>(dst - 32, src - 32); break;
    case 178: memcpy_avx<4>(dst - 178, src - 178);
    case 50: memcpy_avx<1>(dst - 50, src - 50); memcpy_avx<1>(dst - 32, src - 32); break;
    case 179: memcpy_avx<4>(dst - 179, src - 179);
    case 51: memcpy_avx<1>(dst - 51, src - 51); memcpy_avx<1>(dst - 32, src - 32); break;
    case 180: memcpy_avx<4>(dst - 180, src - 180);
    case 52: memcpy_avx<1>(dst - 52, src - 52); memcpy_avx<1>(dst - 32, src - 32); break;
    case 181: memcpy_avx<4>(dst - 181, src - 181);
    case 53: memcpy_avx<1>(dst - 53, src - 53); memcpy_avx<1>(dst - 32, src - 32); break;
    case 182: memcpy_avx<4>(dst - 182, src - 182);
    case 54: memcpy_avx<1>(dst - 54, src - 54); memcpy_avx<1>(dst - 32, src - 32); break;
    case 183: memcpy_avx<4>(dst - 183, src - 183);
    case 55: memcpy_avx<1>(dst - 55, src - 55); memcpy_avx<1>(dst - 32, src - 32); break;
    case 184: memcpy_avx<4>(dst - 184, src - 184);
    case 56: memcpy_avx<1>(dst - 56, src - 56); memcpy_avx<1>(dst - 32, src - 32); break;
    case 185: memcpy_avx<4>(dst - 185, src - 185);
    case 57: memcpy_avx<1>(dst - 57, src - 57); memcpy_avx<1>(dst - 32, src - 32); break;
    case 186: memcpy_avx<4>(dst - 186, src - 186);
    case 58: memcpy_avx<1>(dst - 58, src - 58); memcpy_avx<1>(dst - 32, src - 32); break;
    case 187: memcpy_avx<4>(dst - 187, src - 187);
    case 59: memcpy_avx<1>(dst - 59, src - 59); memcpy_avx<1>(dst - 32, src - 32); break;
    case 188: memcpy_avx<4>(dst - 188, src - 188);
    case 60: memcpy_avx<1>(dst - 60, src - 60); memcpy_avx<1>(dst - 32, src - 32); break;
    case 189: memcpy_avx<4>(dst - 189, src - 189);
    case 61: memcpy_avx<1>(dst - 61, src - 61); memcpy_avx<1>(dst - 32, src - 32); break;
    case 190: memcpy_avx<4>(dst - 190, src - 190);
    case 62: memcpy_avx<1>(dst - 62, src - 62); memcpy_avx<1>(dst - 32, src - 32); break;
    case 191: memcpy_avx<4>(dst - 191, src - 191);
    case 63: memcpy_avx<1>(dst - 63, src - 63); memcpy_avx<1>(dst - 32, src - 32); break;
    case 192: memcpy_avx<4>(dst - 192, src - 192);
    case 64: memcpy_avx<2>(dst - 64, src - 64); break;
    case 193: memcpy_avx<4>(dst - 193, src - 193);
    case 65: memcpy_avx<2>(dst - 65, src - 65); dst[-1] = src[-1]; break;
    case 194: memcpy_avx<4>(dst - 194, src - 194);
    case 66: memcpy_avx<2>(dst - 66, src - 66); *((u16*)(dst - 2)) = *((u16*)(src - 2)); break;
    case 195: memcpy_avx<4>(dst - 195, src - 195);
    case 67: memcpy_avx<2>(dst - 67, src - 67); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 196: memcpy_avx<4>(dst - 196, src - 196);
    case 68: memcpy_avx<2>(dst - 68, src - 68); *((u32*)(dst - 4)) = *((u32*)(src - 4)); break;
    case 197: memcpy_avx<4>(dst - 197, src - 197);
    case 69: memcpy_avx<2>(dst - 69, src - 69); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 198: memcpy_avx<4>(dst - 198, src - 198);
    case 70: memcpy_avx<2>(dst - 70, src - 70); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 199: memcpy_avx<4>(dst - 199, src - 199);
    case 71: memcpy_avx<2>(dst - 71, src - 71); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 200: memcpy_avx<4>(dst - 200, src - 200);
    case 72: memcpy_avx<2>(dst - 72, src - 72); *((u64*)(dst - 8)) = *((u64*)(src - 8)); break;
    case 201: memcpy_avx<4>(dst - 201, src - 201);
    case 73: memcpy_avx<2>(dst - 73, src - 73); memcpy_sse<1>(dst - 16, src - 16); break;
    case 202: memcpy_avx<4>(dst - 202, src - 202);
    case 74: memcpy_avx<2>(dst - 74, src - 74); memcpy_sse<1>(dst - 16, src - 16); break;
    case 203: memcpy_avx<4>(dst - 203, src - 203);
    case 75: memcpy_avx<2>(dst - 75, src - 75); memcpy_sse<1>(dst - 16, src - 16); break;
    case 204: memcpy_avx<4>(dst - 204, src - 204);
    case 76: memcpy_avx<2>(dst - 76, src - 76); memcpy_sse<1>(dst - 16, src - 16); break;
    case 205: memcpy_avx<4>(dst - 205, src - 205);
    case 77: memcpy_avx<2>(dst - 77, src - 77); memcpy_sse<1>(dst - 16, src - 16); break;
    case 206: memcpy_avx<4>(dst - 206, src - 206);
    case 78: memcpy_avx<2>(dst - 78, src - 78); memcpy_sse<1>(dst - 16, src - 16); break;
    case 207: memcpy_avx<4>(dst - 207, src - 207);
    case 79: memcpy_avx<2>(dst - 79, src - 79); memcpy_sse<1>(dst - 16, src - 16); break;
    case 208: memcpy_avx<4>(dst - 208, src - 208);
    case 80: memcpy_avx<2>(dst - 80, src - 80); memcpy_sse<1>(dst - 16, src - 16); break;
    case 209: memcpy_avx<4>(dst - 209, src - 209);
    case 81: memcpy_avx<2>(dst - 81, src - 81); memcpy_avx<1>(dst - 32, src - 32); break;
    case 210: memcpy_avx<4>(dst - 210, src - 210);
    case 82: memcpy_avx<2>(dst - 82, src - 82); memcpy_avx<1>(dst - 32, src - 32); break;
    case 211: memcpy_avx<4>(dst - 211, src - 211);
    case 83: memcpy_avx<2>(dst - 83, src - 83); memcpy_avx<1>(dst - 32, src - 32); break;
    case 212: memcpy_avx<4>(dst - 212, src - 212);
    case 84: memcpy_avx<2>(dst - 84, src - 84); memcpy_avx<1>(dst - 32, src - 32); break;
    case 213: memcpy_avx<4>(dst - 213, src - 213);
    case 85: memcpy_avx<2>(dst - 85, src - 85); memcpy_avx<1>(dst - 32, src - 32); break;
    case 214: memcpy_avx<4>(dst - 214, src - 214);
    case 86: memcpy_avx<2>(dst - 86, src - 86); memcpy_avx<1>(dst - 32, src - 32); break;
    case 215: memcpy_avx<4>(dst - 215, src - 215);
    case 87: memcpy_avx<2>(dst - 87, src - 87); memcpy_avx<1>(dst - 32, src - 32); break;
    case 216: memcpy_avx<4>(dst - 216, src - 216);
    case 88: memcpy_avx<2>(dst - 88, src - 88); memcpy_avx<1>(dst - 32, src - 32); break;
    case 217: memcpy_avx<4>(dst - 217, src - 217);
    case 89: memcpy_avx<2>(dst - 89, src - 89); memcpy_avx<1>(dst - 32, src - 32); break;
    case 218: memcpy_avx<4>(dst - 218, src - 218);
    case 90: memcpy_avx<2>(dst - 90, src - 90); memcpy_avx<1>(dst - 32, src - 32); break;
    case 219: memcpy_avx<4>(dst - 219, src - 219);
    case 91: memcpy_avx<2>(dst - 91, src - 91); memcpy_avx<1>(dst - 32, src - 32); break;
    case 220: memcpy_avx<4>(dst - 220, src - 220);
    case 92: memcpy_avx<2>(dst - 92, src - 92); memcpy_avx<1>(dst - 32, src - 32); break;
    case 221: memcpy_avx<4>(dst - 221, src - 221);
    case 93: memcpy_avx<2>(dst - 93, src - 93); memcpy_avx<1>(dst - 32, src - 32); break;
    case 222: memcpy_avx<4>(dst - 222, src - 222);
    case 94: memcpy_avx<2>(dst - 94, src - 94); memcpy_avx<1>(dst - 32, src - 32); break;
    case 223: memcpy_avx<4>(dst - 223, src - 223);
    case 95: memcpy_avx<2>(dst - 95, src - 95); memcpy_avx<1>(dst - 32, src - 32); break;
    case 224: memcpy_avx<4>(dst - 224, src - 224);
    case 96: memcpy_avx<2>(dst - 96, src - 96); memcpy_avx<1>(dst - 32, src - 32); break;
    case 225: memcpy_avx<4>(dst - 225, src - 225);
    case 97: memcpy_avx<2>(dst - 97, src - 97); memcpy_avx<2>(dst - 64, src - 64); break;
    case 226: memcpy_avx<4>(dst - 226, src - 226);
    case 98: memcpy_avx<2>(dst - 98, src - 98); memcpy_avx<2>(dst - 64, src - 64); break;
    case 227: memcpy_avx<4>(dst - 227, src - 227);
    case 99: memcpy_avx<2>(dst - 99, src - 99); memcpy_avx<2>(dst - 64, src - 64); break;
    case 228: memcpy_avx<4>(dst - 228, src - 228);
    case 100: memcpy_avx<2>(dst - 100, src - 100); memcpy_avx<2>(dst - 64, src - 64); break;
    case 229: memcpy_avx<4>(dst - 229, src - 229);
    case 101: memcpy_avx<2>(dst - 101, src - 101); memcpy_avx<2>(dst - 64, src - 64); break;
    case 230: memcpy_avx<4>(dst - 230, src - 230);
    case 102: memcpy_avx<2>(dst - 102, src - 102); memcpy_avx<2>(dst - 64, src - 64); break;
    case 231: memcpy_avx<4>(dst - 231, src - 231);
    case 103: memcpy_avx<2>(dst - 103, src - 103); memcpy_avx<2>(dst - 64, src - 64); break;
    case 232: memcpy_avx<4>(dst - 232, src - 232);
    case 104: memcpy_avx<2>(dst - 104, src - 104); memcpy_avx<2>(dst - 64, src - 64); break;
    case 233: memcpy_avx<4>(dst - 233, src - 233);
    case 105: memcpy_avx<2>(dst - 105, src - 105); memcpy_avx<2>(dst - 64, src - 64); break;
    case 234: memcpy_avx<4>(dst - 234, src - 234);
    case 106: memcpy_avx<2>(dst - 106, src - 106); memcpy_avx<2>(dst - 64, src - 64); break;
    case 235: memcpy_avx<4>(dst - 235, src - 235);
    case 107: memcpy_avx<2>(dst - 107, src - 107); memcpy_avx<2>(dst - 64, src - 64); break;
    case 236: memcpy_avx<4>(dst - 236, src - 236);
    case 108: memcpy_avx<2>(dst - 108, src - 108); memcpy_avx<2>(dst - 64, src - 64); break;
    case 237: memcpy_avx<4>(dst - 237, src - 237);
    case 109: memcpy_avx<2>(dst - 109, src - 109); memcpy_avx<2>(dst - 64, src - 64); break;
    case 238: memcpy_avx<4>(dst - 238, src - 238);
    case 110: memcpy_avx<2>(dst - 110, src - 110); memcpy_avx<2>(dst - 64, src - 64); break;
    case 239: memcpy_avx<4>(dst - 239, src - 239);
    case 111: memcpy_avx<2>(dst - 111, src - 111); memcpy_avx<2>(dst - 64, src - 64); break;
    case 240: memcpy_avx<4>(dst - 240, src - 240);
    case 112: memcpy_avx<2>(dst - 112, src - 112); memcpy_avx<2>(dst - 64, src - 64); break;
    case 241: memcpy_avx<4>(dst - 241, src - 241);
    case 113: memcpy_avx<2>(dst - 113, src - 113); memcpy_avx<2>(dst - 64, src - 64); break;
    case 242: memcpy_avx<4>(dst - 242, src - 242);
    case 114: memcpy_avx<2>(dst - 114, src - 114); memcpy_avx<2>(dst - 64, src - 64); break;
    case 243: memcpy_avx<4>(dst - 243, src - 243);
    case 115: memcpy_avx<2>(dst - 115, src - 115); memcpy_avx<2>(dst - 64, src - 64); break;
    case 244: memcpy_avx<4>(dst - 244, src - 244);
    case 116: memcpy_avx<2>(dst - 116, src - 116); memcpy_avx<2>(dst - 64, src - 64); break;
    case 245: memcpy_avx<4>(dst - 245, src - 245);
    case 117: memcpy_avx<2>(dst - 117, src - 117); memcpy_avx<2>(dst - 64, src - 64); break;
    case 246: memcpy_avx<4>(dst - 246, src - 246);
    case 118: memcpy_avx<2>(dst - 118, src - 118); memcpy_avx<2>(dst - 64, src - 64); break;
    case 247: memcpy_avx<4>(dst - 247, src - 247);
    case 119: memcpy_avx<2>(dst - 119, src - 119); memcpy_avx<2>(dst - 64, src - 64); break;
    case 248: memcpy_avx<4>(dst - 248, src - 248);
    case 120: memcpy_avx<2>(dst - 120, src - 120); memcpy_avx<2>(dst - 64, src - 64); break;
    case 249: memcpy_avx<4>(dst - 249, src - 249);
    case 121: memcpy_avx<2>(dst - 121, src - 121); memcpy_avx<2>(dst - 64, src - 64); break;
    case 250: memcpy_avx<4>(dst - 250, src - 250);
    case 122: memcpy_avx<2>(dst - 122, src - 122); memcpy_avx<2>(dst - 64, src - 64); break;
    case 251: memcpy_avx<4>(dst - 251, src - 251);
    case 123: memcpy_avx<2>(dst - 123, src - 123); memcpy_avx<2>(dst - 64, src - 64); break;
    case 252: memcpy_avx<4>(dst - 252, src - 252);
    case 124: memcpy_avx<2>(dst - 124, src - 124); memcpy_avx<2>(dst - 64, src - 64); break;
    case 253: memcpy_avx<4>(dst - 253, src - 253);
    case 125: memcpy_avx<2>(dst - 125, src - 125); memcpy_avx<2>(dst - 64, src - 64); break;
    case 254: memcpy_avx<4>(dst - 254, src - 254);
    case 126: memcpy_avx<2>(dst - 126, src - 126); memcpy_avx<2>(dst - 64, src - 64); break;
    case 255: memcpy_avx<4>(dst - 255, src - 255);
    case 127: memcpy_avx<2>(dst - 127, src - 127); memcpy_avx<2>(dst - 64, src - 64); break;
    case 256: memcpy_avx<8>(dst - 256, src - 256); break;
  }
}

/**/
template <bool DST_AVX_ALIGNED = false,
          bool SRC_AVX_ALIGNED = false>
static inline void memcpy(u8* dst, 
                          const u8* src,
                          u64 size)
{
  if (size <= 256) {
    memcpy_small(dst, src, size);
    return;
  }
  const I8* src_v = (const I8*)src;
  I8* dst_v = (I8*)dst;
  if (!(DST_AVX_ALIGNED || Math::is_aligned<32>(dst))) {
    u64 padding = (32 - (((size_t)dst) & 31)) & 31;
    I8_STOREU(dst_v, I8_LOADU(src_v));
    dst_v = (I8*)(dst + padding);
    src_v = (I8*)(src + padding);
    size -= padding;
  }
  u32 loop_count = (size >> 6);
  if (size <= MEGABYTE) {
    if (SRC_AVX_ALIGNED || Math::is_aligned<32>(src_v)) {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 m[2] = { I8_LOAD(src_v), I8_LOAD(src_v + 1) };
        I8_STORE(dst_v, m[0]);
        I8_STORE(dst_v + 1, m[1]);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    } else {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 m[2] = { I8_LOADU(src_v), I8_LOADU(src_v + 1) };
        I8_STORE(dst_v, m[0]);
        I8_STORE(dst_v + 1, m[1]);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    }
  } else {
    if (SRC_AVX_ALIGNED || Math::is_aligned<32>(src_v)) {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 m[2] = { I8_LOAD(src_v), I8_LOAD(src_v + 1) };
        I8_STORE_NOCACHE(dst_v, m[0]);
        I8_STORE_NOCACHE(dst_v + 1, m[1]);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    } else {
      for (u32 r = 0; r < loop_count; ++r) {
        I8 m[2] = { I8_LOADU(src_v), I8_LOADU(src_v + 1) };
        I8_STORE_NOCACHE(dst_v, m[0]);
        I8_STORE_NOCACHE(dst_v + 1, m[1]);
        dst_v += 2;
        src_v += 2;
        size -= 64;
      }
    }
    FENCE();
  }
  src = (u8*)src_v;
  dst = (u8*)dst_v;
  memcpy_small(dst, src, size);
}
}