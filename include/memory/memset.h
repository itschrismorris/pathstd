/* 'memory/memset.h'

  + Writes 'size' bytes of a value into 'dst'.
  + Uses AVX and an optimized jump table for small sizes.
  + Fast path for AVX-aligned dst.

    Path: https://www.path.blog
*/

#pragma once
#include "types.h"
#include "memory/memory.h"
#include "math/math.h"

namespace Pathlib::Memory {

/**/
template <bool DST_AVX_ALIGNED = false>
static inline void memset(u8* dst,
                          const u8 value,
                          u64 size)
{
  if (size <= 256) {
    memset_small(dst, value, size);
    return;
  }
  I8* dst_v = (I8*)dst;
  I8 v = I8_SET1_8(value);
  if (!(DST_AVX_ALIGNED || Math::is_aligned<32>(dst))) {
    u64 padding = (32 - (((u64)dst) & 31)) & 31;
    I8_STOREU(dst_v, v);
    dst_v = (I8*)(dst + padding);
    size -= padding;
  }
  if (size <= (MEGABYTE << 4)) {
    u32 loop_count = (size >> 6);
    for (u32 r = 0; r < loop_count; ++r) {
      I8_STORE(dst_v, v);
      I8_STORE(dst_v + 1, v);
      PREFETCH_NOCACHE(dst_v + 2);
      dst_v += 2;
      size -= 64;
    }
  } else {
    u32 loop_count = (size >> 8);
    for (u32 r = 0; r < loop_count; ++r) {
      I8_STORE_NOCACHE(dst_v, v);
      I8_STORE_NOCACHE(dst_v + 1, v);
      I8_STORE_NOCACHE(dst_v + 2, v);
      I8_STORE_NOCACHE(dst_v + 3, v);
      I8_STORE_NOCACHE(dst_v + 4, v);
      I8_STORE_NOCACHE(dst_v + 5, v);
      I8_STORE_NOCACHE(dst_v + 6, v);
      I8_STORE_NOCACHE(dst_v + 7, v);
      dst_v += 8;
      size -= 256;
    }
    FENCE();
  }
  dst = (u8*)dst_v;
  memset_small(dst, value, size);
}

/**/
template <typename T>
static inline T broadcast_byte(const u8 byte)
{
  if constexpr(sizeof(T) == 1) {
    return byte;
  } else if constexpr(sizeof(T) == 2) {
    return (byte * 0x0101U);
  } else if constexpr (sizeof(T) == 4) {
    return (byte * 0x01010101U);
  } else {
    return (byte * 0x0101010101010101LLU);
  }
}

/**/
template <u32 REGISTER_COUNT>
static inline void memset_sse(u8* dst, 
                              const u8 value) 
{
  I4 v = I4_SET1_8(value);
  I4* dst_v = (I4*)dst;
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I4_STOREU(dst_v + r, v);
  }
}

/**/
template <u32 REGISTER_COUNT>
static inline void memset_avx(u8* dst, 
                              const u8 value) 
{
  I8 v = I8_SET1_8(value);
  I8* dst_v = (I8*)dst;
  #pragma unroll
  for (u32 r = 0; r < REGISTER_COUNT; ++r) {
    I8_STOREU(dst_v + r, v);
  }
}

/**/
static inline void memset_small(u8* dst, 
                                const u8 value, 
                                u64 size)
{
  dst += size;
  switch (size) {
    case 128: memset_avx<4>(dst - 128, value);
    case 0:  break;
    case 129: memset_avx<4>(dst - 129, value);
    case 1: dst[-1] = value; break;
    case 130: memset_avx<4>(dst - 130, value);
    case 2: *((u16*)(dst - 2)) = broadcast_byte<u16>(value); break;
    case 131: memset_avx<4>(dst - 131, value);
    case 3: *((u16*)(dst - 3)) = broadcast_byte<u16>(value); dst[-1] = value; break;
    case 132: memset_avx<4>(dst - 132, value);
    case 4: *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 133: memset_avx<4>(dst - 133, value);
    case 5: *((u32*)(dst - 5)) = broadcast_byte<u32>(value); dst[-1] = value; break;
    case 134: memset_avx<4>(dst - 134, value);
    case 6: *((u32*)(dst - 6)) = broadcast_byte<u32>(value); *((u16*)(dst - 2)) = broadcast_byte<u16>(value); break;
    case 135: memset_avx<4>(dst - 135, value);
    case 7: *((u32*)(dst - 7)) = broadcast_byte<u32>(value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 136: memset_avx<4>(dst - 136, value);
    case 8: *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 137: memset_avx<4>(dst - 137, value);
    case 9: *((u64*)(dst - 9)) = broadcast_byte<u64>(value); dst[-1] = value; break;
    case 138: memset_avx<4>(dst - 138, value);
    case 10: *((u64*)(dst - 10)) = broadcast_byte<u64>(value); *((u16*)(dst - 2)) = broadcast_byte<u16>(value); break;
    case 139: memset_avx<4>(dst - 139, value);
    case 11: *((u64*)(dst - 11)) = broadcast_byte<u64>(value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 140: memset_avx<4>(dst - 140, value);
    case 12: *((u64*)(dst - 12)) = broadcast_byte<u64>(value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 141: memset_avx<4>(dst - 141, value);
    case 13: *((u64*)(dst - 13)) = broadcast_byte<u64>(value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 142: memset_avx<4>(dst - 142, value);
    case 14: *((u64*)(dst - 14)) = broadcast_byte<u64>(value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 143: memset_avx<4>(dst - 143, value);
    case 15: *((u64*)(dst - 15)) = broadcast_byte<u64>(value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 144: memset_avx<4>(dst - 144, value);
    case 16: memset_sse<1>(dst - 16, value); break;
    case 145: memset_avx<4>(dst - 145, value);
    case 17: memset_sse<1>(dst - 17, value); dst[-1] = value; break;
    case 146: memset_avx<4>(dst - 146, value);
    case 18: memset_sse<1>(dst - 18, value); *((u16*)(dst - 2)) = broadcast_byte<u16>(value); break;
    case 147: memset_avx<4>(dst - 147, value);
    case 19: memset_sse<1>(dst - 19, value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 148: memset_avx<4>(dst - 148, value);
    case 20: memset_sse<1>(dst - 20, value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 149: memset_avx<4>(dst - 149, value);
    case 21: memset_sse<1>(dst - 21, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 150: memset_avx<4>(dst - 150, value);
    case 22: memset_sse<1>(dst - 22, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 151: memset_avx<4>(dst - 151, value);
    case 23: memset_sse<1>(dst - 23, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 152: memset_avx<4>(dst - 152, value);
    case 24: memset_sse<1>(dst - 24, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 153: memset_avx<4>(dst - 153, value);
    case 25: memset_sse<1>(dst - 25, value); memset_sse<1>(dst - 16, value); break;
    case 154: memset_avx<4>(dst - 154, value);
    case 26: memset_sse<1>(dst - 26, value); memset_sse<1>(dst - 16, value); break;
    case 155: memset_avx<4>(dst - 155, value);
    case 27: memset_sse<1>(dst - 27, value); memset_sse<1>(dst - 16, value); break;
    case 156: memset_avx<4>(dst - 156, value);
    case 28: memset_sse<1>(dst - 28, value); memset_sse<1>(dst - 16, value); break;
    case 157: memset_avx<4>(dst - 157, value);
    case 29: memset_sse<1>(dst - 29, value); memset_sse<1>(dst - 16, value); break;
    case 158: memset_avx<4>(dst - 158, value);
    case 30: memset_sse<1>(dst - 30, value); memset_sse<1>(dst - 16, value); break;
    case 159: memset_avx<4>(dst - 159, value);
    case 31: memset_sse<1>(dst - 31, value); memset_sse<1>(dst - 16, value); break;
    case 160: memset_avx<4>(dst - 160, value);
    case 32: memset_avx<1>(dst - 32, value); break;
    case 161: memset_avx<4>(dst - 161, value);
    case 33: memset_avx<1>(dst - 33, value); dst[-1] = value; break;
    case 162: memset_avx<4>(dst - 162, value);
    case 34: memset_avx<1>(dst - 34, value); *((u16*)(dst - 2)) = broadcast_byte<u16>(value); break;
    case 163: memset_avx<4>(dst - 163, value);
    case 35: memset_avx<1>(dst - 35, value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 164: memset_avx<4>(dst - 164, value);
    case 36: memset_avx<1>(dst - 36, value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 165: memset_avx<4>(dst - 165, value);
    case 37: memset_avx<1>(dst - 37, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 166: memset_avx<4>(dst - 166, value);
    case 38: memset_avx<1>(dst - 38, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 167: memset_avx<4>(dst - 167, value);
    case 39: memset_avx<1>(dst - 39, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 168: memset_avx<4>(dst - 168, value);
    case 40: memset_avx<1>(dst - 40, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 169: memset_avx<4>(dst - 169, value);
    case 41: memset_avx<1>(dst - 41, value); memset_sse<1>(dst - 16, value); break;
    case 170: memset_avx<4>(dst - 170, value);
    case 42: memset_avx<1>(dst - 42, value); memset_sse<1>(dst - 16, value); break;
    case 171: memset_avx<4>(dst - 171, value);
    case 43: memset_avx<1>(dst - 43, value); memset_sse<1>(dst - 16, value); break;
    case 172: memset_avx<4>(dst - 172, value);
    case 44: memset_avx<1>(dst - 44, value); memset_sse<1>(dst - 16, value); break;
    case 173: memset_avx<4>(dst - 173, value);
    case 45: memset_avx<1>(dst - 45, value); memset_sse<1>(dst - 16, value); break;
    case 174: memset_avx<4>(dst - 174, value);
    case 46: memset_avx<1>(dst - 46, value); memset_sse<1>(dst - 16, value); break;
    case 175: memset_avx<4>(dst - 175, value);
    case 47: memset_avx<1>(dst - 47, value); memset_sse<1>(dst - 16, value); break;
    case 176: memset_avx<4>(dst - 176, value);
    case 48: memset_avx<1>(dst - 48, value); memset_sse<1>(dst - 16, value); break;
    case 177: memset_avx<4>(dst - 177, value);
    case 49: memset_avx<1>(dst - 49, value); memset_avx<1>(dst - 32, value); break;
    case 178: memset_avx<4>(dst - 178, value);
    case 50: memset_avx<1>(dst - 50, value); memset_avx<1>(dst - 32, value); break;
    case 179: memset_avx<4>(dst - 179, value);
    case 51: memset_avx<1>(dst - 51, value); memset_avx<1>(dst - 32, value); break;
    case 180: memset_avx<4>(dst - 180, value);
    case 52: memset_avx<1>(dst - 52, value); memset_avx<1>(dst - 32, value); break;
    case 181: memset_avx<4>(dst - 181, value);
    case 53: memset_avx<1>(dst - 53, value); memset_avx<1>(dst - 32, value); break;
    case 182: memset_avx<4>(dst - 182, value);
    case 54: memset_avx<1>(dst - 54, value); memset_avx<1>(dst - 32, value); break;
    case 183: memset_avx<4>(dst - 183, value);
    case 55: memset_avx<1>(dst - 55, value); memset_avx<1>(dst - 32, value); break;
    case 184: memset_avx<4>(dst - 184, value);
    case 56: memset_avx<1>(dst - 56, value); memset_avx<1>(dst - 32, value); break;
    case 185: memset_avx<4>(dst - 185, value);
    case 57: memset_avx<1>(dst - 57, value); memset_avx<1>(dst - 32, value); break;
    case 186: memset_avx<4>(dst - 186, value);
    case 58: memset_avx<1>(dst - 58, value); memset_avx<1>(dst - 32, value); break;
    case 187: memset_avx<4>(dst - 187, value);
    case 59: memset_avx<1>(dst - 59, value); memset_avx<1>(dst - 32, value); break;
    case 188: memset_avx<4>(dst - 188, value);
    case 60: memset_avx<1>(dst - 60, value); memset_avx<1>(dst - 32, value); break;
    case 189: memset_avx<4>(dst - 189, value);
    case 61: memset_avx<1>(dst - 61, value); memset_avx<1>(dst - 32, value); break;
    case 190: memset_avx<4>(dst - 190, value);
    case 62: memset_avx<1>(dst - 62, value); memset_avx<1>(dst - 32, value); break;
    case 191: memset_avx<4>(dst - 191, value);
    case 63: memset_avx<1>(dst - 63, value); memset_avx<1>(dst - 32, value); break;
    case 192: memset_avx<4>(dst - 192, value);
    case 64: memset_avx<2>(dst - 64, value); break;
    case 193: memset_avx<4>(dst - 193, value);
    case 65: memset_avx<2>(dst - 65, value); dst[-1] = value; break;
    case 194: memset_avx<4>(dst - 194, value);
    case 66: memset_avx<2>(dst - 66, value); *((u16*)(dst - 2)) = broadcast_byte<u16>(value); break;
    case 195: memset_avx<4>(dst - 195, value);
    case 67: memset_avx<2>(dst - 67, value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 196: memset_avx<4>(dst - 196, value);
    case 68: memset_avx<2>(dst - 68, value); *((u32*)(dst - 4)) = broadcast_byte<u32>(value); break;
    case 197: memset_avx<4>(dst - 197, value);
    case 69: memset_avx<2>(dst - 69, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 198: memset_avx<4>(dst - 198, value);
    case 70: memset_avx<2>(dst - 70, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 199: memset_avx<4>(dst - 199, value);
    case 71: memset_avx<2>(dst - 71, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 200: memset_avx<4>(dst - 200, value);
    case 72: memset_avx<2>(dst - 72, value); *((u64*)(dst - 8)) = broadcast_byte<u64>(value); break;
    case 201: memset_avx<4>(dst - 201, value);
    case 73: memset_avx<2>(dst - 73, value); memset_sse<1>(dst - 16, value); break;
    case 202: memset_avx<4>(dst - 202, value);
    case 74: memset_avx<2>(dst - 74, value); memset_sse<1>(dst - 16, value); break;
    case 203: memset_avx<4>(dst - 203, value);
    case 75: memset_avx<2>(dst - 75, value); memset_sse<1>(dst - 16, value); break;
    case 204: memset_avx<4>(dst - 204, value);
    case 76: memset_avx<2>(dst - 76, value); memset_sse<1>(dst - 16, value); break;
    case 205: memset_avx<4>(dst - 205, value);
    case 77: memset_avx<2>(dst - 77, value); memset_sse<1>(dst - 16, value); break;
    case 206: memset_avx<4>(dst - 206, value);
    case 78: memset_avx<2>(dst - 78, value); memset_sse<1>(dst - 16, value); break;
    case 207: memset_avx<4>(dst - 207, value);
    case 79: memset_avx<2>(dst - 79, value); memset_sse<1>(dst - 16, value); break;
    case 208: memset_avx<4>(dst - 208, value);
    case 80: memset_avx<2>(dst - 80, value); memset_sse<1>(dst - 16, value); break;
    case 209: memset_avx<4>(dst - 209, value);
    case 81: memset_avx<2>(dst - 81, value); memset_avx<1>(dst - 32, value); break;
    case 210: memset_avx<4>(dst - 210, value);
    case 82: memset_avx<2>(dst - 82, value); memset_avx<1>(dst - 32, value); break;
    case 211: memset_avx<4>(dst - 211, value);
    case 83: memset_avx<2>(dst - 83, value); memset_avx<1>(dst - 32, value); break;
    case 212: memset_avx<4>(dst - 212, value);
    case 84: memset_avx<2>(dst - 84, value); memset_avx<1>(dst - 32, value); break;
    case 213: memset_avx<4>(dst - 213, value);
    case 85: memset_avx<2>(dst - 85, value); memset_avx<1>(dst - 32, value); break;
    case 214: memset_avx<4>(dst - 214, value);
    case 86: memset_avx<2>(dst - 86, value); memset_avx<1>(dst - 32, value); break;
    case 215: memset_avx<4>(dst - 215, value);
    case 87: memset_avx<2>(dst - 87, value); memset_avx<1>(dst - 32, value); break;
    case 216: memset_avx<4>(dst - 216, value);
    case 88: memset_avx<2>(dst - 88, value); memset_avx<1>(dst - 32, value); break;
    case 217: memset_avx<4>(dst - 217, value);
    case 89: memset_avx<2>(dst - 89, value); memset_avx<1>(dst - 32, value); break;
    case 218: memset_avx<4>(dst - 218, value);
    case 90: memset_avx<2>(dst - 90, value); memset_avx<1>(dst - 32, value); break;
    case 219: memset_avx<4>(dst - 219, value);
    case 91: memset_avx<2>(dst - 91, value); memset_avx<1>(dst - 32, value); break;
    case 220: memset_avx<4>(dst - 220, value);
    case 92: memset_avx<2>(dst - 92, value); memset_avx<1>(dst - 32, value); break;
    case 221: memset_avx<4>(dst - 221, value);
    case 93: memset_avx<2>(dst - 93, value); memset_avx<1>(dst - 32, value); break;
    case 222: memset_avx<4>(dst - 222, value);
    case 94: memset_avx<2>(dst - 94, value); memset_avx<1>(dst - 32, value); break;
    case 223: memset_avx<4>(dst - 223, value);
    case 95: memset_avx<2>(dst - 95, value); memset_avx<1>(dst - 32, value); break;
    case 224: memset_avx<4>(dst - 224, value);
    case 96: memset_avx<2>(dst - 96, value); memset_avx<1>(dst - 32, value); break;
    case 225: memset_avx<4>(dst - 225, value);
    case 97: memset_avx<2>(dst - 97, value); memset_avx<2>(dst - 64, value); break;
    case 226: memset_avx<4>(dst - 226, value);
    case 98: memset_avx<2>(dst - 98, value); memset_avx<2>(dst - 64, value); break;
    case 227: memset_avx<4>(dst - 227, value);
    case 99: memset_avx<2>(dst - 99, value); memset_avx<2>(dst - 64, value); break;
    case 228: memset_avx<4>(dst - 228, value);
    case 100: memset_avx<2>(dst - 100, value); memset_avx<2>(dst - 64, value); break;
    case 229: memset_avx<4>(dst - 229, value);
    case 101: memset_avx<2>(dst - 101, value); memset_avx<2>(dst - 64, value); break;
    case 230: memset_avx<4>(dst - 230, value);
    case 102: memset_avx<2>(dst - 102, value); memset_avx<2>(dst - 64, value); break;
    case 231: memset_avx<4>(dst - 231, value);
    case 103: memset_avx<2>(dst - 103, value); memset_avx<2>(dst - 64, value); break;
    case 232: memset_avx<4>(dst - 232, value);
    case 104: memset_avx<2>(dst - 104, value); memset_avx<2>(dst - 64, value); break;
    case 233: memset_avx<4>(dst - 233, value);
    case 105: memset_avx<2>(dst - 105, value); memset_avx<2>(dst - 64, value); break;
    case 234: memset_avx<4>(dst - 234, value);
    case 106: memset_avx<2>(dst - 106, value); memset_avx<2>(dst - 64, value); break;
    case 235: memset_avx<4>(dst - 235, value);
    case 107: memset_avx<2>(dst - 107, value); memset_avx<2>(dst - 64, value); break;
    case 236: memset_avx<4>(dst - 236, value);
    case 108: memset_avx<2>(dst - 108, value); memset_avx<2>(dst - 64, value); break;
    case 237: memset_avx<4>(dst - 237, value);
    case 109: memset_avx<2>(dst - 109, value); memset_avx<2>(dst - 64, value); break;
    case 238: memset_avx<4>(dst - 238, value);
    case 110: memset_avx<2>(dst - 110, value); memset_avx<2>(dst - 64, value); break;
    case 239: memset_avx<4>(dst - 239, value);
    case 111: memset_avx<2>(dst - 111, value); memset_avx<2>(dst - 64, value); break;
    case 240: memset_avx<4>(dst - 240, value);
    case 112: memset_avx<2>(dst - 112, value); memset_avx<2>(dst - 64, value); break;
    case 241: memset_avx<4>(dst - 241, value);
    case 113: memset_avx<2>(dst - 113, value); memset_avx<2>(dst - 64, value); break;
    case 242: memset_avx<4>(dst - 242, value);
    case 114: memset_avx<2>(dst - 114, value); memset_avx<2>(dst - 64, value); break;
    case 243: memset_avx<4>(dst - 243, value);
    case 115: memset_avx<2>(dst - 115, value); memset_avx<2>(dst - 64, value); break;
    case 244: memset_avx<4>(dst - 244, value);
    case 116: memset_avx<2>(dst - 116, value); memset_avx<2>(dst - 64, value); break;
    case 245: memset_avx<4>(dst - 245, value);
    case 117: memset_avx<2>(dst - 117, value); memset_avx<2>(dst - 64, value); break;
    case 246: memset_avx<4>(dst - 246, value);
    case 118: memset_avx<2>(dst - 118, value); memset_avx<2>(dst - 64, value); break;
    case 247: memset_avx<4>(dst - 247, value);
    case 119: memset_avx<2>(dst - 119, value); memset_avx<2>(dst - 64, value); break;
    case 248: memset_avx<4>(dst - 248, value);
    case 120: memset_avx<2>(dst - 120, value); memset_avx<2>(dst - 64, value); break;
    case 249: memset_avx<4>(dst - 249, value);
    case 121: memset_avx<2>(dst - 121, value); memset_avx<2>(dst - 64, value); break;
    case 250: memset_avx<4>(dst - 250, value);
    case 122: memset_avx<2>(dst - 122, value); memset_avx<2>(dst - 64, value); break;
    case 251: memset_avx<4>(dst - 251, value);
    case 123: memset_avx<2>(dst - 123, value); memset_avx<2>(dst - 64, value); break;
    case 252: memset_avx<4>(dst - 252, value);
    case 124: memset_avx<2>(dst - 124, value); memset_avx<2>(dst - 64, value); break;
    case 253: memset_avx<4>(dst - 253, value);
    case 125: memset_avx<2>(dst - 125, value); memset_avx<2>(dst - 64, value); break;
    case 254: memset_avx<4>(dst - 254, value);
    case 126: memset_avx<2>(dst - 126, value); memset_avx<2>(dst - 64, value); break;
    case 255: memset_avx<4>(dst - 255, value);
    case 127: memset_avx<2>(dst - 127, value); memset_avx<2>(dst - 64, value); break;
    case 256: memset_avx<8>(dst - 256, value); break;
  }
}
}