/* 
  Documentation: https://www.path.blog/docs/strlen.html
*/

#pragma once
#include "types.h"
#include "math/math.h"
#include "memory/memcpy.h"
#include "string/itoa.h"
#include "string/strlen.h"

namespace Pathlib::String {


/**/
static inline utf8* ftoa(f32 value,
                         utf8* buffer)
{
  utf8* icursor = buffer;
  i32 ipart = (i32)value;
  icursor = itoa(ipart, icursor);
  u64 ipart_size = strlen(icursor);
  icursor[ipart_size] = '.';
  u32 fpart = (u32)(Math::abs(value - (f32)ipart) * 10000);
  utf8* fcursor = &icursor[ipart_size + 1];
  if (fpart < 1000) {
    icursor[ipart_size + 1] = '0';
    fcursor += 1;
  }
  utf8* ffcursor = fcursor;
  ffcursor = itoa(fpart, ffcursor);
  Memory::memcpy((u8*)fcursor, (u8*)ffcursor, 4);
  icursor[ipart_size + 5] = '\0';
  return icursor;
}
}