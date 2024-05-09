/* 
  Documentation: https://www.path.blog/docs/from_float.html
*/

#pragma once
#include "types.h"
#include "math/math.h"
#include "memory/memcpy.h"
#include "string/from_int.h"
#include "string/strlen.h"

namespace Pathlib::String {

/**/
template <typename T>
static inline utf8* from_float(T value,
                               utf8* buffer)
{
  f64 v = value;
  if (Math::is_inf(v)) {
    buffer[0] = 'i'; buffer[1] = 'n'; buffer[2] = 'f';
    return buffer;
  } else if (Math::is_nan(v)) {
    buffer[0] = 'n'; buffer[1] = 'a'; buffer[2] = 'n';
    return buffer;
  }
  utf8* output = buffer;
  i32 whole_number = (i32)v;
  output = from_int(whole_number, output);
  u32 whole_number_length = strlen(output);
  output[whole_number_length] = '.';
  u32 fractional_number = (u32)(Math::abs(v - (u32)whole_number) * 1000000.0);
  utf8* fractional_start = &output[whole_number_length + 1];
  i32 divisor = 100000;
  for (u32 f = 0; f < 5; ++f) {
    if (fractional_number < divisor) {
      output[whole_number_length + f + 1] = '0';
      fractional_start += 1;
    }
    divisor /= 10;
  }
  utf8* fraction_string = fractional_start;
  fraction_string = from_int((i32)fractional_number, fraction_string);
  u32 fraction_length = strlen(fraction_string);
  Memory::memcpy((u8*)fractional_start, (u8*)fraction_string, sizeof(utf8) * fraction_length);
  output[whole_number_length + 1 + 6] = '\0';
  return output;
}
}