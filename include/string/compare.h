/* 
  Documentation: https://www.path.blog/docs/size.html
*/

#pragma once
#include "types.h"
#include "string/short_string.h"
#include "math/math.h"

namespace Pathlib::String {

/**/
template <u32 REGISTER_COUNT>
static inline bool compare_sse(const utf8* first,
                               const utf8* second)
{
  u32 bitmask = U32_MAX;
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I4 first_v = I4_LOAD(&first[r * 32]);
    I4 second_v = I4_LOAD(&second[r * 32]);
    bitmask &= I4_MOVEMASK(I4_CMP_EQ(first_v, second_v));
  }
  return (bitmask == U32_MAX);
}

/**/
template <u32 REGISTER_COUNT>
static inline bool compare_avx(const utf8* first,
                               const utf8* second)
{
  u32 bitmask = U32_MAX;
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I8 first_v = I8_LOAD(&first[r * 32]);
    I8 second_v = I8_LOAD(&second[r * 32]);
    bitmask &= I8_MOVEMASK(I8_CMP_EQ(first_v, second_v));
  }
  return (bitmask == U32_MAX);
}


/**/
static inline bool compare_256(const utf8* first,
                               const utf8* second,
                               u64 size)
{
  const utf8* _dst = first + size;
  const utf8* _src = second + size;
  switch (size) {
    case 128: return compare_avx<4>(_dst - 128, _src - 128);
    case 0: return true;
    case 129: if (!compare_avx<4>(_dst - 129, _src - 129)) return false;
    case 1: return _dst[-1] == _src[-1];
    case 130: if (!compare_avx<4>(_dst - 130, _src - 130)) return false;
    case 2: return (*((u16*)(_dst - 2)) == *((u16*)(_src - 2)));
    case 131: if (!compare_avx<4>(_dst - 131, _src - 131)) return false;
    case 3: return ((*((u16*)(_dst - 3)) == *((u16*)(_src - 3))) && (_dst[-1] == _src[-1]));
    case 132: if (!compare_avx<4>(_dst - 132, _src - 132)) return false;
    case 4: return (*((u32*)(_dst - 4)) == *((u32*)(_src - 4)));
    case 133: if (!compare_avx<4>(_dst - 133, _src - 133)) return false;
    case 5: return ((*((u32*)(_dst - 5)) == *((u32*)(_src - 5))) && (_dst[-1] == _src[-1]));
    case 134: if (!compare_avx<4>(_dst - 134, _src - 134)) return false;
    case 6: return ((*((u32*)(_dst - 6)) == *((u32*)(_src - 6))) && (*((u16*)(_dst - 2)) == *((u16*)(_src - 2))));
    case 135: if (!compare_avx<4>(_dst - 135, _src - 135)) return false;
    case 7: return ((*((u32*)(_dst - 7)) = *((u32*)(_src - 7))) && (*((u32*)(_dst - 4)) == *((u32*)(_src - 4))));

    case 136: compare_avx<4>(_dst - 136, _src - 136);
    case 8: *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 137: compare_avx<4>(_dst - 137, _src - 137);
    case 9: *((u64*)(_dst - 9)) = *((u64*)(_src - 9)); _dst[-1] = _src[-1]; break;
    case 138: compare_avx<4>(_dst - 138, _src - 138);
    case 10: *((u64*)(_dst - 10)) = *((u64*)(_src - 10)); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 139: compare_avx<4>(_dst - 139, _src - 139);
    case 11: *((u64*)(_dst - 11)) = *((u64*)(_src - 11)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 140: compare_avx<4>(_dst - 140, _src - 140);
    case 12: *((u64*)(_dst - 12)) = *((u64*)(_src - 12)); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 141: compare_avx<4>(_dst - 141, _src - 141);
    case 13: *((u64*)(_dst - 13)) = *((u64*)(_src - 13)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 142: compare_avx<4>(_dst - 142, _src - 142);
    case 14: *((u64*)(_dst - 14)) = *((u64*)(_src - 14)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 143: compare_avx<4>(_dst - 143, _src - 143);
    case 15: *((u64*)(_dst - 15)) = *((u64*)(_src - 15)); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 144: compare_avx<4>(_dst - 144, _src - 144);
    case 16: compare_sse<1>(_dst - 16, _src - 16); break;
    case 145: compare_avx<4>(_dst - 145, _src - 145);
    case 17: compare_sse<1>(_dst - 17, _src - 17); _dst[-1] = _src[-1]; break;
    case 146: compare_avx<4>(_dst - 146, _src - 146);
    case 18: compare_sse<1>(_dst - 18, _src - 18); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 147: compare_avx<4>(_dst - 147, _src - 147);
    case 19: compare_sse<1>(_dst - 19, _src - 19); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 148: compare_avx<4>(_dst - 148, _src - 148);
    case 20: compare_sse<1>(_dst - 20, _src - 20); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 149: compare_avx<4>(_dst - 149, _src - 149);
    case 21: compare_sse<1>(_dst - 21, _src - 21); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 150: compare_avx<4>(_dst - 150, _src - 150);
    case 22: compare_sse<1>(_dst - 22, _src - 22); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 151: compare_avx<4>(_dst - 151, _src - 151);
    case 23: compare_sse<1>(_dst - 23, _src - 23); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 152: compare_avx<4>(_dst - 152, _src - 152);
    case 24: compare_sse<1>(_dst - 24, _src - 24); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 153: compare_avx<4>(_dst - 153, _src - 153);
    case 25: compare_sse<1>(_dst - 25, _src - 25); compare_sse<1>(_dst - 16, _src - 16); break;
    case 154: compare_avx<4>(_dst - 154, _src - 154);
    case 26: compare_sse<1>(_dst - 26, _src - 26); compare_sse<1>(_dst - 16, _src - 16); break;
    case 155: compare_avx<4>(_dst - 155, _src - 155);
    case 27: compare_sse<1>(_dst - 27, _src - 27); compare_sse<1>(_dst - 16, _src - 16); break;
    case 156: compare_avx<4>(_dst - 156, _src - 156);
    case 28: compare_sse<1>(_dst - 28, _src - 28); compare_sse<1>(_dst - 16, _src - 16); break;
    case 157: compare_avx<4>(_dst - 157, _src - 157);
    case 29: compare_sse<1>(_dst - 29, _src - 29); compare_sse<1>(_dst - 16, _src - 16); break;
    case 158: compare_avx<4>(_dst - 158, _src - 158);
    case 30: compare_sse<1>(_dst - 30, _src - 30); compare_sse<1>(_dst - 16, _src - 16); break;
    case 159: compare_avx<4>(_dst - 159, _src - 159);
    case 31: compare_sse<1>(_dst - 31, _src - 31); compare_sse<1>(_dst - 16, _src - 16); break;
    case 160: compare_avx<4>(_dst - 160, _src - 160);
    case 32: compare_avx<1>(_dst - 32, _src - 32); break;
    case 161: compare_avx<4>(_dst - 161, _src - 161);
    case 33: compare_avx<1>(_dst - 33, _src - 33); _dst[-1] = _src[-1]; break;
    case 162: compare_avx<4>(_dst - 162, _src - 162);
    case 34: compare_avx<1>(_dst - 34, _src - 34); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 163: compare_avx<4>(_dst - 163, _src - 163);
    case 35: compare_avx<1>(_dst - 35, _src - 35); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 164: compare_avx<4>(_dst - 164, _src - 164);
    case 36: compare_avx<1>(_dst - 36, _src - 36); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 165: compare_avx<4>(_dst - 165, _src - 165);
    case 37: compare_avx<1>(_dst - 37, _src - 37); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 166: compare_avx<4>(_dst - 166, _src - 166);
    case 38: compare_avx<1>(_dst - 38, _src - 38); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 167: compare_avx<4>(_dst - 167, _src - 167);
    case 39: compare_avx<1>(_dst - 39, _src - 39); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 168: compare_avx<4>(_dst - 168, _src - 168);
    case 40: compare_avx<1>(_dst - 40, _src - 40); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 169: compare_avx<4>(_dst - 169, _src - 169);
    case 41: compare_avx<1>(_dst - 41, _src - 41); compare_sse<1>(_dst - 16, _src - 16); break;
    case 170: compare_avx<4>(_dst - 170, _src - 170);
    case 42: compare_avx<1>(_dst - 42, _src - 42); compare_sse<1>(_dst - 16, _src - 16); break;
    case 171: compare_avx<4>(_dst - 171, _src - 171);
    case 43: compare_avx<1>(_dst - 43, _src - 43); compare_sse<1>(_dst - 16, _src - 16); break;
    case 172: compare_avx<4>(_dst - 172, _src - 172);
    case 44: compare_avx<1>(_dst - 44, _src - 44); compare_sse<1>(_dst - 16, _src - 16); break;
    case 173: compare_avx<4>(_dst - 173, _src - 173);
    case 45: compare_avx<1>(_dst - 45, _src - 45); compare_sse<1>(_dst - 16, _src - 16); break;
    case 174: compare_avx<4>(_dst - 174, _src - 174);
    case 46: compare_avx<1>(_dst - 46, _src - 46); compare_sse<1>(_dst - 16, _src - 16); break;
    case 175: compare_avx<4>(_dst - 175, _src - 175);
    case 47: compare_avx<1>(_dst - 47, _src - 47); compare_sse<1>(_dst - 16, _src - 16); break;
    case 176: compare_avx<4>(_dst - 176, _src - 176);
    case 48: compare_avx<1>(_dst - 48, _src - 48); compare_sse<1>(_dst - 16, _src - 16); break;
    case 177: compare_avx<4>(_dst - 177, _src - 177);
    case 49: compare_avx<1>(_dst - 49, _src - 49); compare_avx<1>(_dst - 32, _src - 32); break;
    case 178: compare_avx<4>(_dst - 178, _src - 178);
    case 50: compare_avx<1>(_dst - 50, _src - 50); compare_avx<1>(_dst - 32, _src - 32); break;
    case 179: compare_avx<4>(_dst - 179, _src - 179);
    case 51: compare_avx<1>(_dst - 51, _src - 51); compare_avx<1>(_dst - 32, _src - 32); break;
    case 180: compare_avx<4>(_dst - 180, _src - 180);
    case 52: compare_avx<1>(_dst - 52, _src - 52); compare_avx<1>(_dst - 32, _src - 32); break;
    case 181: compare_avx<4>(_dst - 181, _src - 181);
    case 53: compare_avx<1>(_dst - 53, _src - 53); compare_avx<1>(_dst - 32, _src - 32); break;
    case 182: compare_avx<4>(_dst - 182, _src - 182);
    case 54: compare_avx<1>(_dst - 54, _src - 54); compare_avx<1>(_dst - 32, _src - 32); break;
    case 183: compare_avx<4>(_dst - 183, _src - 183);
    case 55: compare_avx<1>(_dst - 55, _src - 55); compare_avx<1>(_dst - 32, _src - 32); break;
    case 184: compare_avx<4>(_dst - 184, _src - 184);
    case 56: compare_avx<1>(_dst - 56, _src - 56); compare_avx<1>(_dst - 32, _src - 32); break;
    case 185: compare_avx<4>(_dst - 185, _src - 185);
    case 57: compare_avx<1>(_dst - 57, _src - 57); compare_avx<1>(_dst - 32, _src - 32); break;
    case 186: compare_avx<4>(_dst - 186, _src - 186);
    case 58: compare_avx<1>(_dst - 58, _src - 58); compare_avx<1>(_dst - 32, _src - 32); break;
    case 187: compare_avx<4>(_dst - 187, _src - 187);
    case 59: compare_avx<1>(_dst - 59, _src - 59); compare_avx<1>(_dst - 32, _src - 32); break;
    case 188: compare_avx<4>(_dst - 188, _src - 188);
    case 60: compare_avx<1>(_dst - 60, _src - 60); compare_avx<1>(_dst - 32, _src - 32); break;
    case 189: compare_avx<4>(_dst - 189, _src - 189);
    case 61: compare_avx<1>(_dst - 61, _src - 61); compare_avx<1>(_dst - 32, _src - 32); break;
    case 190: compare_avx<4>(_dst - 190, _src - 190);
    case 62: compare_avx<1>(_dst - 62, _src - 62); compare_avx<1>(_dst - 32, _src - 32); break;
    case 191: compare_avx<4>(_dst - 191, _src - 191);
    case 63: compare_avx<1>(_dst - 63, _src - 63); compare_avx<1>(_dst - 32, _src - 32); break;
    case 192: compare_avx<4>(_dst - 192, _src - 192);
    case 64: compare_avx<2>(_dst - 64, _src - 64); break;
    case 193: compare_avx<4>(_dst - 193, _src - 193);
    case 65: compare_avx<2>(_dst - 65, _src - 65); _dst[-1] = _src[-1]; break;
    case 194: compare_avx<4>(_dst - 194, _src - 194);
    case 66: compare_avx<2>(_dst - 66, _src - 66); *((u16*)(_dst - 2)) = *((u16*)(_src - 2)); break;
    case 195: compare_avx<4>(_dst - 195, _src - 195);
    case 67: compare_avx<2>(_dst - 67, _src - 67); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 196: compare_avx<4>(_dst - 196, _src - 196);
    case 68: compare_avx<2>(_dst - 68, _src - 68); *((u32*)(_dst - 4)) = *((u32*)(_src - 4)); break;
    case 197: compare_avx<4>(_dst - 197, _src - 197);
    case 69: compare_avx<2>(_dst - 69, _src - 69); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 198: compare_avx<4>(_dst - 198, _src - 198);
    case 70: compare_avx<2>(_dst - 70, _src - 70); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 199: compare_avx<4>(_dst - 199, _src - 199);
    case 71: compare_avx<2>(_dst - 71, _src - 71); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 200: compare_avx<4>(_dst - 200, _src - 200);
    case 72: compare_avx<2>(_dst - 72, _src - 72); *((u64*)(_dst - 8)) = *((u64*)(_src - 8)); break;
    case 201: compare_avx<4>(_dst - 201, _src - 201);
    case 73: compare_avx<2>(_dst - 73, _src - 73); compare_sse<1>(_dst - 16, _src - 16); break;
    case 202: compare_avx<4>(_dst - 202, _src - 202);
    case 74: compare_avx<2>(_dst - 74, _src - 74); compare_sse<1>(_dst - 16, _src - 16); break;
    case 203: compare_avx<4>(_dst - 203, _src - 203);
    case 75: compare_avx<2>(_dst - 75, _src - 75); compare_sse<1>(_dst - 16, _src - 16); break;
    case 204: compare_avx<4>(_dst - 204, _src - 204);
    case 76: compare_avx<2>(_dst - 76, _src - 76); compare_sse<1>(_dst - 16, _src - 16); break;
    case 205: compare_avx<4>(_dst - 205, _src - 205);
    case 77: compare_avx<2>(_dst - 77, _src - 77); compare_sse<1>(_dst - 16, _src - 16); break;
    case 206: compare_avx<4>(_dst - 206, _src - 206);
    case 78: compare_avx<2>(_dst - 78, _src - 78); compare_sse<1>(_dst - 16, _src - 16); break;
    case 207: compare_avx<4>(_dst - 207, _src - 207);
    case 79: compare_avx<2>(_dst - 79, _src - 79); compare_sse<1>(_dst - 16, _src - 16); break;
    case 208: compare_avx<4>(_dst - 208, _src - 208);
    case 80: compare_avx<2>(_dst - 80, _src - 80); compare_sse<1>(_dst - 16, _src - 16); break;
    case 209: compare_avx<4>(_dst - 209, _src - 209);
    case 81: compare_avx<2>(_dst - 81, _src - 81); compare_avx<1>(_dst - 32, _src - 32); break;
    case 210: compare_avx<4>(_dst - 210, _src - 210);
    case 82: compare_avx<2>(_dst - 82, _src - 82); compare_avx<1>(_dst - 32, _src - 32); break;
    case 211: compare_avx<4>(_dst - 211, _src - 211);
    case 83: compare_avx<2>(_dst - 83, _src - 83); compare_avx<1>(_dst - 32, _src - 32); break;
    case 212: compare_avx<4>(_dst - 212, _src - 212);
    case 84: compare_avx<2>(_dst - 84, _src - 84); compare_avx<1>(_dst - 32, _src - 32); break;
    case 213: compare_avx<4>(_dst - 213, _src - 213);
    case 85: compare_avx<2>(_dst - 85, _src - 85); compare_avx<1>(_dst - 32, _src - 32); break;
    case 214: compare_avx<4>(_dst - 214, _src - 214);
    case 86: compare_avx<2>(_dst - 86, _src - 86); compare_avx<1>(_dst - 32, _src - 32); break;
    case 215: compare_avx<4>(_dst - 215, _src - 215);
    case 87: compare_avx<2>(_dst - 87, _src - 87); compare_avx<1>(_dst - 32, _src - 32); break;
    case 216: compare_avx<4>(_dst - 216, _src - 216);
    case 88: compare_avx<2>(_dst - 88, _src - 88); compare_avx<1>(_dst - 32, _src - 32); break;
    case 217: compare_avx<4>(_dst - 217, _src - 217);
    case 89: compare_avx<2>(_dst - 89, _src - 89); compare_avx<1>(_dst - 32, _src - 32); break;
    case 218: compare_avx<4>(_dst - 218, _src - 218);
    case 90: compare_avx<2>(_dst - 90, _src - 90); compare_avx<1>(_dst - 32, _src - 32); break;
    case 219: compare_avx<4>(_dst - 219, _src - 219);
    case 91: compare_avx<2>(_dst - 91, _src - 91); compare_avx<1>(_dst - 32, _src - 32); break;
    case 220: compare_avx<4>(_dst - 220, _src - 220);
    case 92: compare_avx<2>(_dst - 92, _src - 92); compare_avx<1>(_dst - 32, _src - 32); break;
    case 221: compare_avx<4>(_dst - 221, _src - 221);
    case 93: compare_avx<2>(_dst - 93, _src - 93); compare_avx<1>(_dst - 32, _src - 32); break;
    case 222: compare_avx<4>(_dst - 222, _src - 222);
    case 94: compare_avx<2>(_dst - 94, _src - 94); compare_avx<1>(_dst - 32, _src - 32); break;
    case 223: compare_avx<4>(_dst - 223, _src - 223);
    case 95: compare_avx<2>(_dst - 95, _src - 95); compare_avx<1>(_dst - 32, _src - 32); break;
    case 224: compare_avx<4>(_dst - 224, _src - 224);
    case 96: compare_avx<2>(_dst - 96, _src - 96); compare_avx<1>(_dst - 32, _src - 32); break;
    case 225: compare_avx<4>(_dst - 225, _src - 225);
    case 97: compare_avx<2>(_dst - 97, _src - 97); compare_avx<2>(_dst - 64, _src - 64); break;
    case 226: compare_avx<4>(_dst - 226, _src - 226);
    case 98: compare_avx<2>(_dst - 98, _src - 98); compare_avx<2>(_dst - 64, _src - 64); break;
    case 227: compare_avx<4>(_dst - 227, _src - 227);
    case 99: compare_avx<2>(_dst - 99, _src - 99); compare_avx<2>(_dst - 64, _src - 64); break;
    case 228: compare_avx<4>(_dst - 228, _src - 228);
    case 100: compare_avx<2>(_dst - 100, _src - 100); compare_avx<2>(_dst - 64, _src - 64); break;
    case 229: compare_avx<4>(_dst - 229, _src - 229);
    case 101: compare_avx<2>(_dst - 101, _src - 101); compare_avx<2>(_dst - 64, _src - 64); break;
    case 230: compare_avx<4>(_dst - 230, _src - 230);
    case 102: compare_avx<2>(_dst - 102, _src - 102); compare_avx<2>(_dst - 64, _src - 64); break;
    case 231: compare_avx<4>(_dst - 231, _src - 231);
    case 103: compare_avx<2>(_dst - 103, _src - 103); compare_avx<2>(_dst - 64, _src - 64); break;
    case 232: compare_avx<4>(_dst - 232, _src - 232);
    case 104: compare_avx<2>(_dst - 104, _src - 104); compare_avx<2>(_dst - 64, _src - 64); break;
    case 233: compare_avx<4>(_dst - 233, _src - 233);
    case 105: compare_avx<2>(_dst - 105, _src - 105); compare_avx<2>(_dst - 64, _src - 64); break;
    case 234: compare_avx<4>(_dst - 234, _src - 234);
    case 106: compare_avx<2>(_dst - 106, _src - 106); compare_avx<2>(_dst - 64, _src - 64); break;
    case 235: compare_avx<4>(_dst - 235, _src - 235);
    case 107: compare_avx<2>(_dst - 107, _src - 107); compare_avx<2>(_dst - 64, _src - 64); break;
    case 236: compare_avx<4>(_dst - 236, _src - 236);
    case 108: compare_avx<2>(_dst - 108, _src - 108); compare_avx<2>(_dst - 64, _src - 64); break;
    case 237: compare_avx<4>(_dst - 237, _src - 237);
    case 109: compare_avx<2>(_dst - 109, _src - 109); compare_avx<2>(_dst - 64, _src - 64); break;
    case 238: compare_avx<4>(_dst - 238, _src - 238);
    case 110: compare_avx<2>(_dst - 110, _src - 110); compare_avx<2>(_dst - 64, _src - 64); break;
    case 239: compare_avx<4>(_dst - 239, _src - 239);
    case 111: compare_avx<2>(_dst - 111, _src - 111); compare_avx<2>(_dst - 64, _src - 64); break;
    case 240: compare_avx<4>(_dst - 240, _src - 240);
    case 112: compare_avx<2>(_dst - 112, _src - 112); compare_avx<2>(_dst - 64, _src - 64); break;
    case 241: compare_avx<4>(_dst - 241, _src - 241);
    case 113: compare_avx<2>(_dst - 113, _src - 113); compare_avx<2>(_dst - 64, _src - 64); break;
    case 242: compare_avx<4>(_dst - 242, _src - 242);
    case 114: compare_avx<2>(_dst - 114, _src - 114); compare_avx<2>(_dst - 64, _src - 64); break;
    case 243: compare_avx<4>(_dst - 243, _src - 243);
    case 115: compare_avx<2>(_dst - 115, _src - 115); compare_avx<2>(_dst - 64, _src - 64); break;
    case 244: compare_avx<4>(_dst - 244, _src - 244);
    case 116: compare_avx<2>(_dst - 116, _src - 116); compare_avx<2>(_dst - 64, _src - 64); break;
    case 245: compare_avx<4>(_dst - 245, _src - 245);
    case 117: compare_avx<2>(_dst - 117, _src - 117); compare_avx<2>(_dst - 64, _src - 64); break;
    case 246: compare_avx<4>(_dst - 246, _src - 246);
    case 118: compare_avx<2>(_dst - 118, _src - 118); compare_avx<2>(_dst - 64, _src - 64); break;
    case 247: compare_avx<4>(_dst - 247, _src - 247);
    case 119: compare_avx<2>(_dst - 119, _src - 119); compare_avx<2>(_dst - 64, _src - 64); break;
    case 248: compare_avx<4>(_dst - 248, _src - 248);
    case 120: compare_avx<2>(_dst - 120, _src - 120); compare_avx<2>(_dst - 64, _src - 64); break;
    case 249: compare_avx<4>(_dst - 249, _src - 249);
    case 121: compare_avx<2>(_dst - 121, _src - 121); compare_avx<2>(_dst - 64, _src - 64); break;
    case 250: compare_avx<4>(_dst - 250, _src - 250);
    case 122: compare_avx<2>(_dst - 122, _src - 122); compare_avx<2>(_dst - 64, _src - 64); break;
    case 251: compare_avx<4>(_dst - 251, _src - 251);
    case 123: compare_avx<2>(_dst - 123, _src - 123); compare_avx<2>(_dst - 64, _src - 64); break;
    case 252: compare_avx<4>(_dst - 252, _src - 252);
    case 124: compare_avx<2>(_dst - 124, _src - 124); compare_avx<2>(_dst - 64, _src - 64); break;
    case 253: compare_avx<4>(_dst - 253, _src - 253);
    case 125: compare_avx<2>(_dst - 125, _src - 125); compare_avx<2>(_dst - 64, _src - 64); break;
    case 254: compare_avx<4>(_dst - 254, _src - 254);
    case 126: compare_avx<2>(_dst - 126, _src - 126); compare_avx<2>(_dst - 64, _src - 64); break;
    case 255: compare_avx<4>(_dst - 255, _src - 255);
    case 127: compare_avx<2>(_dst - 127, _src - 127); compare_avx<2>(_dst - 64, _src - 64); break;
    case 256: compare_avx<8>(_dst - 256, _src - 256); break;
  }
}

/**/
template <u32 FIRST_ALIGNED_32 = false, 
          u64 SECOND_ALIGNED_32 = false>
static inline bool compare(const utf8* first,
                           const utf8* second,
                           u64 first_size = 0,
                           u64 second_size = 0)
{
  first_size = (first_size > 0) ? first_size : String::size_of(first);
  second_size = (second_size > 0) ? second_size : String::size_of(second);
  if (first_size != second_size) {
    return false;
  }
  u32 avx_count = first_size / 32;
  u32 bitmask = U32_MAX;
  #pragma unroll
  for (u32 c = 0; c < avx_count; ++c) {
    I8 first_v, second_v;
    if constexpr (FIRST_ALIGNED_32) {
      first_v = I8_LOAD(&first[c * 32]);
    } else {
      first_v = I8_LOADU(&first[c * 32]);
    }
    if constexpr (SECOND_ALIGNED_32) {
      second_v = I8_LOAD(&second[c * 32]);
    } else {
      second_v = I8_LOADU(&second[c * 32]);
    }
    bitmask &= I8_MOVEMASK(I8_CMP_EQ(first_v, second_v));
  }
  if (bitmask != U32_MAX) {
    return false;
  }
  for (u32 c = avx_count * 32; c < first_size; ++c) {
    if (first[c] != second[c]) {
      return false;
    }
  }
  return true;
}
}