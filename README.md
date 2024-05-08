# pathlib

* Pathlib is a small and highly optimized standard library, used in the Path game engine.
* No dependency on any standard library (libc, libc++, etc.).
* Written in C++, utilizing AVX2 instructions.
* Only dependency is mimalloc, for memory management: https://github.com/microsoft/mimalloc.
* Requires Windows and the AVX2 instruction set.
* Documentation at www.path.blog.

---------------------------

- pathlib
  - memory
    - <b>['memcpy.h'](https://path.blog/docs/memcpy.html)</b>
    - <b>'avx_memcmp'</b>
    - <b>['memset.h'](https://path.blog/docs/memset.html)</b>
  - string
    - <b>['strlen.h'](https://path.blog/docs/strlen.html)</b>
    - <b>'stack_string.h'</b>
  - containers
    - <b>'stack_vector.h'</b>
    - <b>'hashmap.h'</b>
    - <b>'pool.h'</b>
    - <b>'pools.h'</b>
    - <b>'heap.h'</b>
    - <b>'virtual_heap.h'</b>
    - <b>'spsc_queue.h'</b>

---------------------------

\* Optimized hash-prospector results: https://github.com/skeeto/hash-prospector/issues/19#issuecomment-1120105785
