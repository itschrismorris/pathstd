#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "path/std/string/avx_strlen.h"

int32_t main()
{
  /** avx_strlen.h **/
  const char* example_string = "Hello world, this is a test string.";
  size_t length = Path::Std::String::avx_strlen(example_string);
  size_t strlen_length = strlen(example_string);
  if (length != strlen_length) {
    printf("Error: avx_strlen length did not match standard library's strlen result.\n");
  } else {
    printf("String: '%s' contains %llu characters.\n", example_string, length);
  }
  /******************/

  return 0;
}