/* 
  Documentation: https://www.path.blog/docs/size.html
*/

#pragma once
#include "pathlib/types/types.h"
#include "pathlib/types/string/short_string.h"
#include "pathlib/math/math.h"

namespace Pathlib {
namespace String {
namespace _Internal {

//---
template <u32 REGISTER_COUNT>
static inline bool compare_sse(const utf8* first,
                               const utf8* second)
{
  u32 bitmask = Types::U16_MAX;
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I4 first_v = I4_LOADU(&first[r * 16]);
    I4 second_v = I4_LOADU(&second[r * 16]);
    bitmask &= I4_MOVEMASK(I4_CMP_EQ(first_v, second_v));
    if (bitmask != Types::U16_MAX) {
      return false;
    }
  }
  return true;
}

//---
template <u32 REGISTER_COUNT>
static inline bool compare_avx(const utf8* first,
                               const utf8* second)
{
  u32 bitmask = Types::U32_MAX;
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I8 first_v = I8_LOADU(&first[r * 32]);
    I8 second_v = I8_LOADU(&second[r * 32]);
    bitmask &= I8_MOVEMASK(I8_CMP_EQ(first_v, second_v));
    if (bitmask != Types::U32_MAX) {
      return false;
    }
  }
  return true;
}

//---
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
    case 3: return (((*((u16*)(_dst - 3))) == *((u16*)(_src - 3))) && (_dst[-1] == _src[-1]));
    case 132: if (!compare_avx<4>(_dst - 132, _src - 132)) return false;
    case 4: return (*((u32*)(_dst - 4)) == *((u32*)(_src - 4)));
    case 133: if (!compare_avx<4>(_dst - 133, _src - 133)) return false;
    case 5: return (((*((u32*)(_dst - 5)) == *((u32*)(_src - 5)))) && (_dst[-1] == _src[-1]));
    case 134: if (!compare_avx<4>(_dst - 134, _src - 134)) return false;
    case 6: return (((*((u32*)(_dst - 6)) == *((u32*)(_src - 6)))) && (*((u16*)(_dst - 2)) == *((u16*)(_src - 2))));
    case 135: if (!compare_avx<4>(_dst - 135, _src - 135)) return false;
    case 7: return (((*((u32*)(_dst - 7)) = *((u32*)(_src - 7)))) && (*((u32*)(_dst - 4)) == *((u32*)(_src - 4))));
    case 136: if (!compare_avx<4>(_dst - 136, _src - 136)) return false;
    case 8: return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 137: if (!compare_avx<4>(_dst - 137, _src - 137)) return false;
    case 9: return (((*((u64*)(_dst - 9)) == *((u64*)(_src - 9))) && (_dst[-1] == _src[-1])));
    case 138: if (!compare_avx<4>(_dst - 138, _src - 138)) return false;
    case 10: return (((*((u64*)(_dst - 10)) == *((u64*)(_src - 10))) && (*((u16*)(_dst - 2)) == *((u16*)(_src - 2)))));
    case 139: if (!compare_avx<4>(_dst - 139, _src - 139)) return false;
    case 11: return (((*((u64*)(_dst - 11)) == *((u64*)(_src - 11))) && *((u32*)(_dst - 4)) == *((u32*)(_src - 4))));
    case 140: if (!compare_avx<4>(_dst - 140, _src - 140)) return false;
    case 12: return (((*((u64*)(_dst - 12)) == *((u64*)(_src - 12))) && *((u32*)(_dst - 4)) == *((u32*)(_src - 4))));
    case 141: if (!compare_avx<4>(_dst - 141, _src - 141)) return false;
    case 13: return (((*((u64*)(_dst - 13)) == *((u64*)(_src - 13))) && *((u64*)(_dst - 8)) == *((u64*)(_src - 8))));
    case 142: if (!compare_avx<4>(_dst - 142, _src - 142)) return false;
    case 14: return (((*((u64*)(_dst - 14)) == *((u64*)(_src - 14))) && *((u64*)(_dst - 8)) == *((u64*)(_src - 8))));
    case 143: if (!compare_avx<4>(_dst - 143, _src - 143)) return false;
    case 15: return (((*((u64*)(_dst - 15)) == *((u64*)(_src - 15))) && *((u64*)(_dst - 8)) == *((u64*)(_src - 8))));
    case 144: if (!compare_avx<4>(_dst - 144, _src - 144)) return false;
    case 16: return compare_sse<1>(_dst - 16, _src - 16);
    case 145: if (!compare_avx<4>(_dst - 145, _src - 145)) return false;
    case 17: if (!compare_sse<1>(_dst - 17, _src - 17)) return false; return (_dst[-1] == _src[-1]);
    case 146: if (!compare_avx<4>(_dst - 146, _src - 146)) return false;
    case 18: if (!compare_sse<1>(_dst - 18, _src - 18)) return false; return (*((u16*)(_dst - 2)) == *((u16*)(_src - 2)));
    case 147: if (!compare_avx<4>(_dst - 147, _src - 147)) return false;
    case 19: if (!compare_sse<1>(_dst - 19, _src - 19)) return false; return (*((u32*)(_dst - 4)) == *((u32*)(_src - 4)));
    case 148: if (!compare_avx<4>(_dst - 148, _src - 148)) return false;
    case 20: if (!compare_sse<1>(_dst - 20, _src - 20)) return false; return (*((u32*)(_dst - 4)) == *((u32*)(_src - 4)));
    case 149: if (!compare_avx<4>(_dst - 149, _src - 149)) return false;
    case 21: if (!compare_sse<1>(_dst - 21, _src - 21)) return false; return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 150: if (!compare_avx<4>(_dst - 150, _src - 150)) return false;
    case 22: if (!compare_sse<1>(_dst - 22, _src - 22)) return false; return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 151: if (!compare_avx<4>(_dst - 151, _src - 151)) return false;
    case 23: if (!compare_sse<1>(_dst - 23, _src - 23)) return false; return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 152: if (!compare_avx<4>(_dst - 152, _src - 152)) return false;
    case 24: if (!compare_sse<1>(_dst - 24, _src - 24)) return false; return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 153: if (!compare_avx<4>(_dst - 153, _src - 153)) return false;
    case 25: if (!compare_sse<1>(_dst - 25, _src - 25)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 154: if (!compare_avx<4>(_dst - 154, _src - 154)) return false;
    case 26: if (!compare_sse<1>(_dst - 26, _src - 26)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 155: if (!compare_avx<4>(_dst - 155, _src - 155)) return false;
    case 27: if (!compare_sse<1>(_dst - 27, _src - 27)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 156: if (!compare_avx<4>(_dst - 156, _src - 156)) return false;
    case 28: if (!compare_sse<1>(_dst - 28, _src - 28)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 157: if (!compare_avx<4>(_dst - 157, _src - 157)) return false;
    case 29: if (!compare_sse<1>(_dst - 29, _src - 29)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 158: if (!compare_avx<4>(_dst - 158, _src - 158)) return false;
    case 30: if (!compare_sse<1>(_dst - 30, _src - 30)) return false; return  compare_sse<1>(_dst - 16, _src - 16);
    case 159: if (!compare_avx<4>(_dst - 159, _src - 159)) return false;
    case 31: if (!compare_sse<1>(_dst - 31, _src - 31)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 160: if (!compare_avx<4>(_dst - 160, _src - 160)) return false;
    case 32: return compare_avx<1>(_dst - 32, _src - 32);
    case 161: if (!compare_avx<4>(_dst - 161, _src - 161)) return false;
    case 33: if (!compare_avx<1>(_dst - 33, _src - 33)) return false; return (_dst[-1] == _src[-1]);
    case 162: if (!compare_avx<4>(_dst - 162, _src - 162)) return false;
    case 34: if (!compare_avx<1>(_dst - 34, _src - 34)) return false; return (*((u16*)(_dst - 2)) == *((u16*)(_src - 2)));
    case 163: if (!compare_avx<4>(_dst - 163, _src - 163)) return false;
    case 35: if (!compare_avx<1>(_dst - 35, _src - 35)) return false; return (*((u32*)(_dst - 4)) == *((u32*)(_src - 4)));
    case 164: if (!compare_avx<4>(_dst - 164, _src - 164)) return false;
    case 36: if (!compare_avx<1>(_dst - 36, _src - 36)) return false; return (*((u32*)(_dst - 4)) == *((u32*)(_src - 4)));
    case 165: if (!compare_avx<4>(_dst - 165, _src - 165)) return false;
    case 37: if (!compare_avx<1>(_dst - 37, _src - 37)) return false; return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 166: if (!compare_avx<4>(_dst - 166, _src - 166)) return false;
    case 38: if (!compare_avx<1>(_dst - 38, _src - 38)) return false; return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 167: if (!compare_avx<4>(_dst - 167, _src - 167)) return false;
    case 39: if (!compare_avx<1>(_dst - 39, _src - 39)) return false; return (*((u64*)(_dst - 8)) == *((u64*)(_src - 8)));
    case 168: if (!compare_avx<4>(_dst - 168, _src - 168)) return false;
    case 40: if (!compare_avx<1>(_dst - 40, _src - 40)) return false; return (*((u64*)(_dst - 8)) = *((u64*)(_src - 8)));
    case 169: if (!compare_avx<4>(_dst - 169, _src - 169)) return false;
    case 41: if (!compare_avx<1>(_dst - 41, _src - 41)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 170: if (!compare_avx<4>(_dst - 170, _src - 170)) return false;
    case 42: if (!compare_avx<1>(_dst - 42, _src - 42)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 171: if (!compare_avx<4>(_dst - 171, _src - 171)) return false;
    case 43: if (!compare_avx<1>(_dst - 43, _src - 43)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 172: if (!compare_avx<4>(_dst - 172, _src - 172)) return false;
    case 44: if (!compare_avx<1>(_dst - 44, _src - 44)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 173: if (!compare_avx<4>(_dst - 173, _src - 173)) return false;
    case 45: if (!compare_avx<1>(_dst - 45, _src - 45)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 174: if (!compare_avx<4>(_dst - 174, _src - 174)) return false;
    case 46: if (!compare_avx<1>(_dst - 46, _src - 46)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 175: if (!compare_avx<4>(_dst - 175, _src - 175)) return false;
    case 47: if (!compare_avx<1>(_dst - 47, _src - 47)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 176: if (!compare_avx<4>(_dst - 176, _src - 176)) return false;
    case 48: if (!compare_avx<1>(_dst - 48, _src - 48)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 177: if (!compare_avx<4>(_dst - 177, _src - 177)) return false;
    case 49: if (!compare_avx<1>(_dst - 49, _src - 49)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 178: if (!compare_avx<4>(_dst - 178, _src - 178)) return false;
    case 50: if (!compare_avx<1>(_dst - 50, _src - 50)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 179: if (!compare_avx<4>(_dst - 179, _src - 179)) return false;
    case 51: if (!compare_avx<1>(_dst - 51, _src - 51)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 180: if (!compare_avx<4>(_dst - 180, _src - 180)) return false;
    case 52: if (!compare_avx<1>(_dst - 52, _src - 52)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 181: if (!compare_avx<4>(_dst - 181, _src - 181)) return false;
    case 53: if (!compare_avx<1>(_dst - 53, _src - 53)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 182: if (!compare_avx<4>(_dst - 182, _src - 182)) return false;
    case 54: if (!compare_avx<1>(_dst - 54, _src - 54)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 183: if (!compare_avx<4>(_dst - 183, _src - 183)) return false;
    case 55: if (!compare_avx<1>(_dst - 55, _src - 55)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 184: if (!compare_avx<4>(_dst - 184, _src - 184)) return false;
    case 56: if (!compare_avx<1>(_dst - 56, _src - 56)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 185: if (!compare_avx<4>(_dst - 185, _src - 185)) return false;
    case 57: if (!compare_avx<1>(_dst - 57, _src - 57)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 186: if (!compare_avx<4>(_dst - 186, _src - 186)) return false;
    case 58: if (!compare_avx<1>(_dst - 58, _src - 58)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 187: if (!compare_avx<4>(_dst - 187, _src - 187)) return false;
    case 59: if (!compare_avx<1>(_dst - 59, _src - 59)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 188: if (!compare_avx<4>(_dst - 188, _src - 188)) return false;
    case 60: if (!compare_avx<1>(_dst - 60, _src - 60)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 189: if (!compare_avx<4>(_dst - 189, _src - 189)) return false;
    case 61: if (!compare_avx<1>(_dst - 61, _src - 61)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 190: if (!compare_avx<4>(_dst - 190, _src - 190)) return false;
    case 62: if (!compare_avx<1>(_dst - 62, _src - 62)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 191: if (!compare_avx<4>(_dst - 191, _src - 191)) return false;
    case 63: if (!compare_avx<1>(_dst - 63, _src - 63)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 192: if (!compare_avx<4>(_dst - 192, _src - 192)) return false;
    case 64: return compare_avx<2>(_dst - 64, _src - 64);
    case 193: if (!compare_avx<4>(_dst - 193, _src - 193)) return false;
    case 65: if (!compare_avx<2>(_dst - 65, _src - 65)) return false; return (_dst[-1] == _src[-1]);
    case 194: if (!compare_avx<4>(_dst - 194, _src - 194)) return false;
    case 66: if (!compare_avx<2>(_dst - 66, _src - 66)) return false; return (*((u16*)(_dst - 2)) = *((u16*)(_src - 2)));
    case 195: if (!compare_avx<4>(_dst - 195, _src - 195)) return false;
    case 67: if (!compare_avx<2>(_dst - 67, _src - 67)) return false; return (*((u32*)(_dst - 4)) = *((u32*)(_src - 4)));
    case 196: if (!compare_avx<4>(_dst - 196, _src - 196)) return false;
    case 68: if (!compare_avx<2>(_dst - 68, _src - 68)) return false; return (*((u32*)(_dst - 4)) = *((u32*)(_src - 4)));
    case 197: if (!compare_avx<4>(_dst - 197, _src - 197)) return false;
    case 69: if (!compare_avx<2>(_dst - 69, _src - 69)) return false; return (*((u64*)(_dst - 8)) = *((u64*)(_src - 8)));
    case 198: if (!compare_avx<4>(_dst - 198, _src - 198)) return false;
    case 70: if (!compare_avx<2>(_dst - 70, _src - 70)) return false; return (*((u64*)(_dst - 8)) = *((u64*)(_src - 8)));
    case 199: if (!compare_avx<4>(_dst - 199, _src - 199)) return false;
    case 71: if (!compare_avx<2>(_dst - 71, _src - 71)) return false; return (*((u64*)(_dst - 8)) = *((u64*)(_src - 8)));
    case 200: if (!compare_avx<4>(_dst - 200, _src - 200)) return false;
    case 72: if (!compare_avx<2>(_dst - 72, _src - 72)) return false; return (*((u64*)(_dst - 8)) = *((u64*)(_src - 8)));
    case 201: if (!compare_avx<4>(_dst - 201, _src - 201)) return false;
    case 73: if (!compare_avx<2>(_dst - 73, _src - 73)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 202: if (!compare_avx<4>(_dst - 202, _src - 202)) return false;
    case 74: if (!compare_avx<2>(_dst - 74, _src - 74)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 203: if (!compare_avx<4>(_dst - 203, _src - 203)) return false;
    case 75: if (!compare_avx<2>(_dst - 75, _src - 75)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 204: if (!compare_avx<4>(_dst - 204, _src - 204)) return false;
    case 76: if (!compare_avx<2>(_dst - 76, _src - 76)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 205: if (!compare_avx<4>(_dst - 205, _src - 205)) return false;
    case 77: if (!compare_avx<2>(_dst - 77, _src - 77)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 206: if (!compare_avx<4>(_dst - 206, _src - 206)) return false;
    case 78: if (!compare_avx<2>(_dst - 78, _src - 78)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 207: if (!compare_avx<4>(_dst - 207, _src - 207)) return false;
    case 79: if (!compare_avx<2>(_dst - 79, _src - 79)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 208: if (!compare_avx<4>(_dst - 208, _src - 208)) return false;
    case 80: if (!compare_avx<2>(_dst - 80, _src - 80)) return false; return compare_sse<1>(_dst - 16, _src - 16);
    case 209: if (!compare_avx<4>(_dst - 209, _src - 209)) return false;
    case 81: if (!compare_avx<2>(_dst - 81, _src - 81)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 210: if (!compare_avx<4>(_dst - 210, _src - 210)) return false;
    case 82: if (!compare_avx<2>(_dst - 82, _src - 82)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 211: if (!compare_avx<4>(_dst - 211, _src - 211)) return false;
    case 83: if (!compare_avx<2>(_dst - 83, _src - 83)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 212: if (!compare_avx<4>(_dst - 212, _src - 212)) return false;
    case 84: if (!compare_avx<2>(_dst - 84, _src - 84)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 213: if (!compare_avx<4>(_dst - 213, _src - 213)) return false;
    case 85: if (!compare_avx<2>(_dst - 85, _src - 85)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 214: if (!compare_avx<4>(_dst - 214, _src - 214)) return false;
    case 86: if (!compare_avx<2>(_dst - 86, _src - 86)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 215: if (!compare_avx<4>(_dst - 215, _src - 215)) return false;
    case 87: if (!compare_avx<2>(_dst - 87, _src - 87)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 216: if (!compare_avx<4>(_dst - 216, _src - 216)) return false;
    case 88: if (!compare_avx<2>(_dst - 88, _src - 88)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 217: if (!compare_avx<4>(_dst - 217, _src - 217)) return false;
    case 89: if (!compare_avx<2>(_dst - 89, _src - 89)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 218: if (!compare_avx<4>(_dst - 218, _src - 218)) return false;
    case 90: if (!compare_avx<2>(_dst - 90, _src - 90)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 219: if (!compare_avx<4>(_dst - 219, _src - 219)) return false;
    case 91: if (!compare_avx<2>(_dst - 91, _src - 91)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 220: if (!compare_avx<4>(_dst - 220, _src - 220)) return false;
    case 92: if (!compare_avx<2>(_dst - 92, _src - 92)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 221: if (!compare_avx<4>(_dst - 221, _src - 221)) return false;
    case 93: if (!compare_avx<2>(_dst - 93, _src - 93)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 222: if (!compare_avx<4>(_dst - 222, _src - 222)) return false;
    case 94: if (!compare_avx<2>(_dst - 94, _src - 94)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 223: if (!compare_avx<4>(_dst - 223, _src - 223)) return false;
    case 95: if (!compare_avx<2>(_dst - 95, _src - 95)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 224: if (!compare_avx<4>(_dst - 224, _src - 224)) return false;
    case 96: if (!compare_avx<2>(_dst - 96, _src - 96)) return false; return compare_avx<1>(_dst - 32, _src - 32);
    case 225: if (!compare_avx<4>(_dst - 225, _src - 225)) return false;
    case 97: if (!compare_avx<2>(_dst - 97, _src - 97)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 226: if (!compare_avx<4>(_dst - 226, _src - 226)) return false;
    case 98: if (!compare_avx<2>(_dst - 98, _src - 98)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 227: if (!compare_avx<4>(_dst - 227, _src - 227)) return false;
    case 99: if (!compare_avx<2>(_dst - 99, _src - 99)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 228: if (!compare_avx<4>(_dst - 228, _src - 228)) return false;
    case 100: if (!compare_avx<2>(_dst - 100, _src - 100)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 229: if (!compare_avx<4>(_dst - 229, _src - 229)) return false;
    case 101: if (!compare_avx<2>(_dst - 101, _src - 101)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 230: if (!compare_avx<4>(_dst - 230, _src - 230)) return false;
    case 102: if (!compare_avx<2>(_dst - 102, _src - 102)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 231: if (!compare_avx<4>(_dst - 231, _src - 231)) return false;
    case 103: if (!compare_avx<2>(_dst - 103, _src - 103)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 232: if (!compare_avx<4>(_dst - 232, _src - 232)) return false;
    case 104: if (!compare_avx<2>(_dst - 104, _src - 104)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 233: if (!compare_avx<4>(_dst - 233, _src - 233)) return false;
    case 105: if (!compare_avx<2>(_dst - 105, _src - 105)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 234: if (!compare_avx<4>(_dst - 234, _src - 234)) return false;
    case 106: if (!compare_avx<2>(_dst - 106, _src - 106)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 235: if (!compare_avx<4>(_dst - 235, _src - 235)) return false;
    case 107: if (!compare_avx<2>(_dst - 107, _src - 107)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 236: if (!compare_avx<4>(_dst - 236, _src - 236)) return false;
    case 108: if (!compare_avx<2>(_dst - 108, _src - 108)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 237: if (!compare_avx<4>(_dst - 237, _src - 237)) return false;
    case 109: if (!compare_avx<2>(_dst - 109, _src - 109)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 238: if (!compare_avx<4>(_dst - 238, _src - 238)) return false;
    case 110: if (!compare_avx<2>(_dst - 110, _src - 110)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 239: if (!compare_avx<4>(_dst - 239, _src - 239)) return false;
    case 111: if (!compare_avx<2>(_dst - 111, _src - 111)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 240: if (!compare_avx<4>(_dst - 240, _src - 240)) return false;
    case 112: if (!compare_avx<2>(_dst - 112, _src - 112)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 241: if (!compare_avx<4>(_dst - 241, _src - 241)) return false;
    case 113: if (!compare_avx<2>(_dst - 113, _src - 113)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 242: if (!compare_avx<4>(_dst - 242, _src - 242)) return false;
    case 114: if (!compare_avx<2>(_dst - 114, _src - 114)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 243: if (!compare_avx<4>(_dst - 243, _src - 243)) return false;
    case 115: if (!compare_avx<2>(_dst - 115, _src - 115)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 244: if (!compare_avx<4>(_dst - 244, _src - 244)) return false;
    case 116: if (!compare_avx<2>(_dst - 116, _src - 116)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 245: if (!compare_avx<4>(_dst - 245, _src - 245)) return false;
    case 117: if (!compare_avx<2>(_dst - 117, _src - 117)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 246: if (!compare_avx<4>(_dst - 246, _src - 246)) return false;
    case 118: if (!compare_avx<2>(_dst - 118, _src - 118)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 247: if (!compare_avx<4>(_dst - 247, _src - 247)) return false;
    case 119: if (!compare_avx<2>(_dst - 119, _src - 119)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 248: if (!compare_avx<4>(_dst - 248, _src - 248)) return false;
    case 120: if (!compare_avx<2>(_dst - 120, _src - 120)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 249: if (!compare_avx<4>(_dst - 249, _src - 249)) return false;
    case 121: if (!compare_avx<2>(_dst - 121, _src - 121)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 250: if (!compare_avx<4>(_dst - 250, _src - 250)) return false;
    case 122: if (!compare_avx<2>(_dst - 122, _src - 122)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 251: if (!compare_avx<4>(_dst - 251, _src - 251)) return false;
    case 123: if (!compare_avx<2>(_dst - 123, _src - 123)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 252: if (!compare_avx<4>(_dst - 252, _src - 252)) return false;
    case 124: if (!compare_avx<2>(_dst - 124, _src - 124)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 253: if (!compare_avx<4>(_dst - 253, _src - 253)) return false;
    case 125: if (!compare_avx<2>(_dst - 125, _src - 125)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 254: if (!compare_avx<4>(_dst - 254, _src - 254)) return false;
    case 126: if (!compare_avx<2>(_dst - 126, _src - 126)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 255: if (!compare_avx<4>(_dst - 255, _src - 255)) return false;
    case 127: if (!compare_avx<2>(_dst - 127, _src - 127)) return false; return compare_avx<2>(_dst - 64, _src - 64);
    case 256: return compare_avx<8>(_dst - 256, _src - 256);
    default: return false;
  }
}
}

//---
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
  if (first_size <= 256) {
    return compare_256(first, second, first_size);
  }
  u32 avx_count = first_size / 32;
  u32 bitmask = Types::U32_MAX;
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
    if (bitmask != Types::U32_MAX) {
      return false;
    }
  }
  u64 leftover = first_size - (avx_count << 5);
  return compare_256(&first[avx_count << 5], &second[avx_count << 5], leftover);
}
}
}