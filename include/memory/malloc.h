/*
  Documentation: https://www.path.blog/docs/malloc.html
*/
#pragma once
#include "third_party/mimalloc/mimalloc.h"
#include "types.h"

#define PAGE_SIZE 0x1000
#define KILOBYTE (1024)
#define MEGABYTE (1024 * 1024)
#define GIGABYTE (1024 * 1024 * 1024)

namespace Pathlib::Memory {

  /**/
  void initiate();
  void shutdown();
  void* malloc(u64 size,
               const utf8* name = nullptr);
  void* realloc(void* ptr,
                u64 size,
                const utf8* name = nullptr);
  void free(void** ptr);
}