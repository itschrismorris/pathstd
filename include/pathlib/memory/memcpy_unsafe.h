/*
  Documentation: https://www.path.blog/docs/memcpy.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/memory/memory.h"
#include "pathlib/math/math.h"
#include "pathlib/math/simd_math.h"

namespace Pathlib {
namespace _Internal {

//---
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

//---
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

//---
static inline void memcpy_256(void* dst, 
                              const void* src,
                              u64 size)
{
  u8* _dst = (u8*)dst + size;
  u8* _src = (u8*)src + size;
  switch (size) {
    case 128: memcpy_avx<4>(_dst - 128, _src - 128);
    case 0:  break;
    case 129: memcpy_avx<4>(_dst - 129, _src - 129);
    case 1: _dst[-1] = _src[-1]; break;
    case 130: memcpy_avx<4>(_dst - 130, _src - 130);
    case 2: *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 131: memcpy_avx<4>(_dst - 131, _src - 131);
    case 3: *((u16*)(_dst - 3)) = *((u16*)(_src - 3)); _dst[-1] = _src[-1]; break;
    case 132: memcpy_avx<4>(_dst - 132, _src - 132);
    case 4: *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 133: memcpy_avx<4>(_dst - 133, _src - 133);
    case 5: *((u32*)(_dst - 5)) = *((u32*)(_src - 5)); _dst[-1] = _src[-1]; break;
    case 134: memcpy_avx<4>(_dst - 134, _src - 134);
    case 6: *((u32*)(_dst - 6)) = *((u32*)(_src - 6)); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 135: memcpy_avx<4>(_dst - 135, _src - 135);
    case 7: *((u32*)(_dst - 7)) = *((u32*)(_src - 7)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 136: memcpy_avx<4>(_dst - 136, _src - 136);
    case 8: *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 137: memcpy_avx<4>(_dst - 137, _src - 137);
    case 9: *((u64*)(_dst - 9)) = *((u64*)(_src - 9)); _dst[-1] = _src[-1]; break;
    case 138: memcpy_avx<4>(_dst - 138, _src - 138);
    case 10: *((u64*)(_dst - 10)) = *((u64*)(_src - 10)); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 139: memcpy_avx<4>(_dst - 139, _src - 139);
    case 11: *((u64*)(_dst - 11)) = *((u64*)(_src - 11)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 140: memcpy_avx<4>(_dst - 140, _src - 140);
    case 12: *((u64*)(_dst - 12)) = *((u64*)(_src - 12)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 141: memcpy_avx<4>(_dst - 141, _src - 141);
    case 13: *((u64*)(_dst - 13)) = *((u64*)(_src - 13)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 142: memcpy_avx<4>(_dst - 142, _src - 142);
    case 14: *((u64*)(_dst - 14)) = *((u64*)(_src - 14)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 143: memcpy_avx<4>(_dst - 143, _src - 143);
    case 15: *((u64*)(_dst - 15)) = *((u64*)(_src - 15)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 144: memcpy_avx<4>(_dst - 144, _src - 144);
    case 16: memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 145: memcpy_avx<4>(_dst - 145, _src - 145);
    case 17: memcpy_sse<1>(_dst - 17, _src - 17); _dst[-1] = _src[-1]; break;
    case 146: memcpy_avx<4>(_dst - 146, _src - 146);
    case 18: memcpy_sse<1>(_dst - 18, _src - 18); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 147: memcpy_avx<4>(_dst - 147, _src - 147);
    case 19: memcpy_sse<1>(_dst - 19, _src - 19); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 148: memcpy_avx<4>(_dst - 148, _src - 148);
    case 20: memcpy_sse<1>(_dst - 20, _src - 20); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 149: memcpy_avx<4>(_dst - 149, _src - 149);
    case 21: memcpy_sse<1>(_dst - 21, _src - 21); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 150: memcpy_avx<4>(_dst - 150, _src - 150);
    case 22: memcpy_sse<1>(_dst - 22, _src - 22); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 151: memcpy_avx<4>(_dst - 151, _src - 151);
    case 23: memcpy_sse<1>(_dst - 23, _src - 23); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 152: memcpy_avx<4>(_dst - 152, _src - 152);
    case 24: memcpy_sse<1>(_dst - 24, _src - 24); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 153: memcpy_avx<4>(_dst - 153, _src - 153);
    case 25: memcpy_sse<1>(_dst - 25, _src - 25); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 154: memcpy_avx<4>(_dst - 154, _src - 154);
    case 26: memcpy_sse<1>(_dst - 26, _src - 26); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 155: memcpy_avx<4>(_dst - 155, _src - 155);
    case 27: memcpy_sse<1>(_dst - 27, _src - 27); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 156: memcpy_avx<4>(_dst - 156, _src - 156);
    case 28: memcpy_sse<1>(_dst - 28, _src - 28); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 157: memcpy_avx<4>(_dst - 157, _src - 157);
    case 29: memcpy_sse<1>(_dst - 29, _src - 29); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 158: memcpy_avx<4>(_dst - 158, _src - 158);
    case 30: memcpy_sse<1>(_dst - 30, _src - 30); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 159: memcpy_avx<4>(_dst - 159, _src - 159);
    case 31: memcpy_sse<1>(_dst - 31, _src - 31); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 160: memcpy_avx<4>(_dst - 160, _src - 160);
    case 32: memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 161: memcpy_avx<4>(_dst - 161, _src - 161);
    case 33: memcpy_avx<1>(_dst - 33, _src - 33); _dst[-1] = _src[-1]; break;
    case 162: memcpy_avx<4>(_dst - 162, _src - 162);
    case 34: memcpy_avx<1>(_dst - 34, _src - 34); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 163: memcpy_avx<4>(_dst - 163, _src - 163);
    case 35: memcpy_avx<1>(_dst - 35, _src - 35); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 164: memcpy_avx<4>(_dst - 164, _src - 164);
    case 36: memcpy_avx<1>(_dst - 36, _src - 36); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 165: memcpy_avx<4>(_dst - 165, _src - 165);
    case 37: memcpy_avx<1>(_dst - 37, _src - 37); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 166: memcpy_avx<4>(_dst - 166, _src - 166);
    case 38: memcpy_avx<1>(_dst - 38, _src - 38); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 167: memcpy_avx<4>(_dst - 167, _src - 167);
    case 39: memcpy_avx<1>(_dst - 39, _src - 39); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 168: memcpy_avx<4>(_dst - 168, _src - 168);
    case 40: memcpy_avx<1>(_dst - 40, _src - 40); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 169: memcpy_avx<4>(_dst - 169, _src - 169);
    case 41: memcpy_avx<1>(_dst - 41, _src - 41); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 170: memcpy_avx<4>(_dst - 170, _src - 170);
    case 42: memcpy_avx<1>(_dst - 42, _src - 42); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 171: memcpy_avx<4>(_dst - 171, _src - 171);
    case 43: memcpy_avx<1>(_dst - 43, _src - 43); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 172: memcpy_avx<4>(_dst - 172, _src - 172);
    case 44: memcpy_avx<1>(_dst - 44, _src - 44); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 173: memcpy_avx<4>(_dst - 173, _src - 173);
    case 45: memcpy_avx<1>(_dst - 45, _src - 45); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 174: memcpy_avx<4>(_dst - 174, _src - 174);
    case 46: memcpy_avx<1>(_dst - 46, _src - 46); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 175: memcpy_avx<4>(_dst - 175, _src - 175);
    case 47: memcpy_avx<1>(_dst - 47, _src - 47); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 176: memcpy_avx<4>(_dst - 176, _src - 176);
    case 48: memcpy_avx<1>(_dst - 48, _src - 48); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 177: memcpy_avx<4>(_dst - 177, _src - 177);
    case 49: memcpy_avx<1>(_dst - 49, _src - 49); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 178: memcpy_avx<4>(_dst - 178, _src - 178);
    case 50: memcpy_avx<1>(_dst - 50, _src - 50); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 179: memcpy_avx<4>(_dst - 179, _src - 179);
    case 51: memcpy_avx<1>(_dst - 51, _src - 51); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 180: memcpy_avx<4>(_dst - 180, _src - 180);
    case 52: memcpy_avx<1>(_dst - 52, _src - 52); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 181: memcpy_avx<4>(_dst - 181, _src - 181);
    case 53: memcpy_avx<1>(_dst - 53, _src - 53); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 182: memcpy_avx<4>(_dst - 182, _src - 182);
    case 54: memcpy_avx<1>(_dst - 54, _src - 54); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 183: memcpy_avx<4>(_dst - 183, _src - 183);
    case 55: memcpy_avx<1>(_dst - 55, _src - 55); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 184: memcpy_avx<4>(_dst - 184, _src - 184);
    case 56: memcpy_avx<1>(_dst - 56, _src - 56); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 185: memcpy_avx<4>(_dst - 185, _src - 185);
    case 57: memcpy_avx<1>(_dst - 57, _src - 57); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 186: memcpy_avx<4>(_dst - 186, _src - 186);
    case 58: memcpy_avx<1>(_dst - 58, _src - 58); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 187: memcpy_avx<4>(_dst - 187, _src - 187);
    case 59: memcpy_avx<1>(_dst - 59, _src - 59); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 188: memcpy_avx<4>(_dst - 188, _src - 188);
    case 60: memcpy_avx<1>(_dst - 60, _src - 60); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 189: memcpy_avx<4>(_dst - 189, _src - 189);
    case 61: memcpy_avx<1>(_dst - 61, _src - 61); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 190: memcpy_avx<4>(_dst - 190, _src - 190);
    case 62: memcpy_avx<1>(_dst - 62, _src - 62); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 191: memcpy_avx<4>(_dst - 191, _src - 191);
    case 63: memcpy_avx<1>(_dst - 63, _src - 63); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 192: memcpy_avx<4>(_dst - 192, _src - 192);
    case 64: memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 193: memcpy_avx<4>(_dst - 193, _src - 193);
    case 65: memcpy_avx<2>(_dst - 65, _src - 65); _dst[-1] = _src[-1]; break;
    case 194: memcpy_avx<4>(_dst - 194, _src - 194);
    case 66: memcpy_avx<2>(_dst - 66, _src - 66); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 195: memcpy_avx<4>(_dst - 195, _src - 195);
    case 67: memcpy_avx<2>(_dst - 67, _src - 67); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 196: memcpy_avx<4>(_dst - 196, _src - 196);
    case 68: memcpy_avx<2>(_dst - 68, _src - 68); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 197: memcpy_avx<4>(_dst - 197, _src - 197);
    case 69: memcpy_avx<2>(_dst - 69, _src - 69); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 198: memcpy_avx<4>(_dst - 198, _src - 198);
    case 70: memcpy_avx<2>(_dst - 70, _src - 70); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 199: memcpy_avx<4>(_dst - 199, _src - 199);
    case 71: memcpy_avx<2>(_dst - 71, _src - 71); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 200: memcpy_avx<4>(_dst - 200, _src - 200);
    case 72: memcpy_avx<2>(_dst - 72, _src - 72); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 201: memcpy_avx<4>(_dst - 201, _src - 201);
    case 73: memcpy_avx<2>(_dst - 73, _src - 73); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 202: memcpy_avx<4>(_dst - 202, _src - 202);
    case 74: memcpy_avx<2>(_dst - 74, _src - 74); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 203: memcpy_avx<4>(_dst - 203, _src - 203);
    case 75: memcpy_avx<2>(_dst - 75, _src - 75); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 204: memcpy_avx<4>(_dst - 204, _src - 204);
    case 76: memcpy_avx<2>(_dst - 76, _src - 76); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 205: memcpy_avx<4>(_dst - 205, _src - 205);
    case 77: memcpy_avx<2>(_dst - 77, _src - 77); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 206: memcpy_avx<4>(_dst - 206, _src - 206);
    case 78: memcpy_avx<2>(_dst - 78, _src - 78); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 207: memcpy_avx<4>(_dst - 207, _src - 207);
    case 79: memcpy_avx<2>(_dst - 79, _src - 79); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 208: memcpy_avx<4>(_dst - 208, _src - 208);
    case 80: memcpy_avx<2>(_dst - 80, _src - 80); memcpy_sse<1>(_dst - 16, _src - 16); break;
    case 209: memcpy_avx<4>(_dst - 209, _src - 209);
    case 81: memcpy_avx<2>(_dst - 81, _src - 81); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 210: memcpy_avx<4>(_dst - 210, _src - 210);
    case 82: memcpy_avx<2>(_dst - 82, _src - 82); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 211: memcpy_avx<4>(_dst - 211, _src - 211);
    case 83: memcpy_avx<2>(_dst - 83, _src - 83); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 212: memcpy_avx<4>(_dst - 212, _src - 212);
    case 84: memcpy_avx<2>(_dst - 84, _src - 84); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 213: memcpy_avx<4>(_dst - 213, _src - 213);
    case 85: memcpy_avx<2>(_dst - 85, _src - 85); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 214: memcpy_avx<4>(_dst - 214, _src - 214);
    case 86: memcpy_avx<2>(_dst - 86, _src - 86); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 215: memcpy_avx<4>(_dst - 215, _src - 215);
    case 87: memcpy_avx<2>(_dst - 87, _src - 87); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 216: memcpy_avx<4>(_dst - 216, _src - 216);
    case 88: memcpy_avx<2>(_dst - 88, _src - 88); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 217: memcpy_avx<4>(_dst - 217, _src - 217);
    case 89: memcpy_avx<2>(_dst - 89, _src - 89); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 218: memcpy_avx<4>(_dst - 218, _src - 218);
    case 90: memcpy_avx<2>(_dst - 90, _src - 90); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 219: memcpy_avx<4>(_dst - 219, _src - 219);
    case 91: memcpy_avx<2>(_dst - 91, _src - 91); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 220: memcpy_avx<4>(_dst - 220, _src - 220);
    case 92: memcpy_avx<2>(_dst - 92, _src - 92); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 221: memcpy_avx<4>(_dst - 221, _src - 221);
    case 93: memcpy_avx<2>(_dst - 93, _src - 93); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 222: memcpy_avx<4>(_dst - 222, _src - 222);
    case 94: memcpy_avx<2>(_dst - 94, _src - 94); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 223: memcpy_avx<4>(_dst - 223, _src - 223);
    case 95: memcpy_avx<2>(_dst - 95, _src - 95); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 224: memcpy_avx<4>(_dst - 224, _src - 224);
    case 96: memcpy_avx<2>(_dst - 96, _src - 96); memcpy_avx<1>(_dst - 32, _src - 32); break;
    case 225: memcpy_avx<4>(_dst - 225, _src - 225);
    case 97: memcpy_avx<2>(_dst - 97, _src - 97); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 226: memcpy_avx<4>(_dst - 226, _src - 226);
    case 98: memcpy_avx<2>(_dst - 98, _src - 98); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 227: memcpy_avx<4>(_dst - 227, _src - 227);
    case 99: memcpy_avx<2>(_dst - 99, _src - 99); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 228: memcpy_avx<4>(_dst - 228, _src - 228);
    case 100: memcpy_avx<2>(_dst - 100, _src - 100); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 229: memcpy_avx<4>(_dst - 229, _src - 229);
    case 101: memcpy_avx<2>(_dst - 101, _src - 101); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 230: memcpy_avx<4>(_dst - 230, _src - 230);
    case 102: memcpy_avx<2>(_dst - 102, _src - 102); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 231: memcpy_avx<4>(_dst - 231, _src - 231);
    case 103: memcpy_avx<2>(_dst - 103, _src - 103); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 232: memcpy_avx<4>(_dst - 232, _src - 232);
    case 104: memcpy_avx<2>(_dst - 104, _src - 104); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 233: memcpy_avx<4>(_dst - 233, _src - 233);
    case 105: memcpy_avx<2>(_dst - 105, _src - 105); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 234: memcpy_avx<4>(_dst - 234, _src - 234);
    case 106: memcpy_avx<2>(_dst - 106, _src - 106); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 235: memcpy_avx<4>(_dst - 235, _src - 235);
    case 107: memcpy_avx<2>(_dst - 107, _src - 107); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 236: memcpy_avx<4>(_dst - 236, _src - 236);
    case 108: memcpy_avx<2>(_dst - 108, _src - 108); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 237: memcpy_avx<4>(_dst - 237, _src - 237);
    case 109: memcpy_avx<2>(_dst - 109, _src - 109); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 238: memcpy_avx<4>(_dst - 238, _src - 238);
    case 110: memcpy_avx<2>(_dst - 110, _src - 110); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 239: memcpy_avx<4>(_dst - 239, _src - 239);
    case 111: memcpy_avx<2>(_dst - 111, _src - 111); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 240: memcpy_avx<4>(_dst - 240, _src - 240);
    case 112: memcpy_avx<2>(_dst - 112, _src - 112); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 241: memcpy_avx<4>(_dst - 241, _src - 241);
    case 113: memcpy_avx<2>(_dst - 113, _src - 113); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 242: memcpy_avx<4>(_dst - 242, _src - 242);
    case 114: memcpy_avx<2>(_dst - 114, _src - 114); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 243: memcpy_avx<4>(_dst - 243, _src - 243);
    case 115: memcpy_avx<2>(_dst - 115, _src - 115); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 244: memcpy_avx<4>(_dst - 244, _src - 244);
    case 116: memcpy_avx<2>(_dst - 116, _src - 116); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 245: memcpy_avx<4>(_dst - 245, _src - 245);
    case 117: memcpy_avx<2>(_dst - 117, _src - 117); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 246: memcpy_avx<4>(_dst - 246, _src - 246);
    case 118: memcpy_avx<2>(_dst - 118, _src - 118); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 247: memcpy_avx<4>(_dst - 247, _src - 247);
    case 119: memcpy_avx<2>(_dst - 119, _src - 119); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 248: memcpy_avx<4>(_dst - 248, _src - 248);
    case 120: memcpy_avx<2>(_dst - 120, _src - 120); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 249: memcpy_avx<4>(_dst - 249, _src - 249);
    case 121: memcpy_avx<2>(_dst - 121, _src - 121); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 250: memcpy_avx<4>(_dst - 250, _src - 250);
    case 122: memcpy_avx<2>(_dst - 122, _src - 122); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 251: memcpy_avx<4>(_dst - 251, _src - 251);
    case 123: memcpy_avx<2>(_dst - 123, _src - 123); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 252: memcpy_avx<4>(_dst - 252, _src - 252);
    case 124: memcpy_avx<2>(_dst - 124, _src - 124); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 253: memcpy_avx<4>(_dst - 253, _src - 253);
    case 125: memcpy_avx<2>(_dst - 125, _src - 125); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 254: memcpy_avx<4>(_dst - 254, _src - 254);
    case 126: memcpy_avx<2>(_dst - 126, _src - 126); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 255: memcpy_avx<4>(_dst - 255, _src - 255);
    case 127: memcpy_avx<2>(_dst - 127, _src - 127); memcpy_avx<2>(_dst - 64, _src - 64); break;
    case 256: memcpy_avx<8>(_dst - 256, _src - 256); break;
  }
}
}

//---
template <bool DST_ALIGNED_32 = false,
          bool SRC_ALIGNED_32 = false>
static inline void memcpy_unsafe(void* dst,
                                 const void* src,
                                 u64 size)
{
  if (size <= 256) {
    _Internal::memcpy_256(dst, src, size);
    return;
  }
  I8* src_v = (I8*)src;
  I8* dst_v = (I8*)dst;
  if (!(DST_ALIGNED_32 || Math::is_aligned<32>(dst))) {
    u64 padding = (32 - (((u64)dst) & 31)) & 31;
    I8_STOREU(dst_v, I8_LOADU(src_v));
    dst_v = (I8*)((u8*)dst + padding);
    src_v = (I8*)((u8*)src + padding);
    size -= padding;
  }
  u32 loop_count = (size >> 6);
  if (size <= Memory::MEGABYTE) {
    if (SRC_ALIGNED_32 || Math::is_aligned<32>(src_v)) {
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
    if (SRC_ALIGNED_32 || Math::is_aligned<32>(src_v)) {
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
  _Internal::memcpy_256((u8*)dst_v, (u8*)src_v, size);
}
}