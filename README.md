# Pathlib

* Pathlib is a highly optimized library of common functions and containers for scripting in [Path](https://path.blog).
* Written in C++20, with full unicode support, and zero use of any standard library (libc, libc++, etc.).
* Only dependency is mimalloc, for memory management: https://github.com/microsoft/mimalloc.
* Requires Windows, C++20, and the AVX2 instruction set.

<b>Note: when needed, efficiency is chosen over "programmer safety".</b> Many of these functions are used in tight loops,
where additional branching to check for every possible bad argument can make a difference. If you need additional safety, check
your arguments before passing them in.

---------------------------

- pathlib/include
  - <b>['types.h'](https://path.blog/docs/types.html)</b>
  - memory
    - <b>['memcpy.h'](https://path.blog/docs/memcpy.html)</b>
    - <b>['memset.h'](https://path.blog/docs/memset.html)</b>
  - string
    - <b>['local_string.h'](https://path.blog/docs/local_string.html)</b>
    - <b>['strlen.h'](https://path.blog/docs/strlen.html)</b>
    - <b>['from_int.h'](https://path.blog/docs/from_int.html)</b>
    - <b>['from_float.h'](https://path.blog/docs/from_float.html)</b>
  - math
    - <b>['math.h'](https://path.blog/docs/math.html)</b>
    - <b>['simd_math.h'](https://path.blog/docs/simd_math.html)</b>
  - win32
    - <b>['console.h'](https://path.blog/docs/console.html)</b>
