# pathlib

* This library is used in the Path game engine itself, as well as for runtime scripting within the engine.
* It provides a build of libc++ for standard library use, as well as Path's lightweight, AVX-optimized containers and routines.
* Memory allocations use with mimalloc library: https://github.com/microsoft/mimalloc.
* For Windows x86_64 platforms that support the AVX2 instruction set.
* Documentation at www.path.blog.

---------------------------

- path
  - std
    - string
      - <b>['avx_strlen.h'](https://path.blog/docs/avx_strlen.html)</b>
        - <i>AVX optimized replacement for strlen().</i>
      - <b>'stack_string.h'</b>
        - <i>AVX optimized short strings held on the stack.</i>
    - containers
      - <b>'stack_vector.h'</b>
        - <i>Optimized for short vectors held on the stack. Uses "swap with end" for fast removals.</b></i>
      - <b>'hashmap.h'</b>
        - <i>Heavily optimized integer hash function*, open-addressed, relocating (Robinhood), not thread-safe, no duplicate keys. Key/Value pairs stored contiguously for fast iteration.</i>
      - <b>'pool.h'</b>
        - <i>Object pool with internal freelist.</i>
      - <b>'pools.h'</b>
        - <i>Vector of 'pool.h' to allow dynamically resizing object pools.</i>
      - <b>'heap.h'</b>
        - <i>Basic compacting heap that informs allocations of changes in their location.</i>
      - <b>'virtual_heap.h'</b>
        - <i>Same as heap but with no actual internal storage. Used to manage heaps that exist in GPU memory only.</i>
      - <b>'spsc_queue.h'</b>
        - <i>Fast lock-free single-producer, single-consumer queue.</i>
    - memory
      - <b>'avx_memcpy'</b>
        - <i>AVX optimized replacement for memcpy().</i>
      - <b>'avx_memcmp'</b>
        - <i>AVX optimized replacement for memcmp().</i>
      - <b>'avx_memset'</b>
        - <i>AVX optimized replacement for memset().</i>

---------------------------

\* Optimized hash-prospector results: https://github.com/skeeto/hash-prospector/issues/19#issuecomment-1120105785
