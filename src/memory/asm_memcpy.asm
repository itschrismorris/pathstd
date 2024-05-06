includelib msvcrt.lib
extern printf: proc

;.data
;  message byte "Debug reached here", 0
;lea rcx, message
;call printf

.code
asm_memcpy_aligned PROC

  ; rcx == dst
  ; rdx == src
  ; r8  == size
  ; rax == return

  mov rax, rcx                                 ; Move dst into rax register.

  cmp r8, 64                                   ; if (size > 64) _above_64();
  ja _above_64
  cmp r8, 16                                   ; if (size >= 16) _16_to_64();
  jae _16_to_64
  ; Falls into (0-15).

_0_to_15:
  cmp r8, 4                                    ; if (size <= 4) _0_to_4();
  jbe _0_to_4      
  cmp r8, 8                                    ; if (size <= 8) _5_to_8();
  jbe _5_to_8
  ; Falls into (9-15).

_9_to_15:
  mov rcx, qword ptr [rdx]                    ; Load quad-word (8 bytes) from src.
  mov qword ptr [rax], rcx                    ; Store into dst.
  mov rcx, qword ptr [rdx + r8 - 8]           ; Load quad-word (8 bytes) from end of src.
  mov qword ptr [rax + r8 - 8], rcx           ; Store into end of dst.
  ret

_0_to_4:
  cmp r8, 0                                   ; if (size == 0) return;
  je _return
  mov cl, byte ptr [rdx]                      ; Load 1 byte from src.
  mov byte ptr [rax], cl                      ; Store into dst.
  mov cl, byte ptr [rdx + r8 - 1]             ; Load 1 byte from end of src.
  mov byte ptr [rax + r8 - 1], cl             ; Store into end of dst.
  cmp r8, 2                                   ; if (size <= 2) return;
  jbe _return
  mov cl, byte ptr [rdx + 1]                  ; Load second byte from src.
  mov byte ptr [rax + 1], cl                  ; Store into dst.
  mov cl, byte ptr [rdx + 2]                  ; Load third byte from src.
  mov byte ptr [rax + 2], cl                  ; Store into dst.
  ret

_16_to_64:
  cmp r8, 32                                  ; if  (size <= 32) _16_to_32();
  jbe _16_to_32
  ; Falls into (32-64).

_32_to_64:
  vmovdqa ymm0, ymmword ptr [rdx]             ; Load AVX register from src.
  vmovdqa ymmword ptr [rax], ymm0             ; Store into dst.
  vmovdqa ymm0, ymmword ptr [rdx + r8 - 32]   ; Load AVX register from end of src.
  vmovdqa ymmword ptr [rax + r8 - 32], ymm0   ; Store into end of dst.
  vzeroupper                                  ; Clean up registers.
  ret

_16_to_32:
  movups xmm0, xmmword ptr [rdx]             ; Load SSE register (16 bytes) from src.
  movups xmmword ptr [rax], xmm0             ; Store into dst.
  movups xmm0, xmmword ptr [rdx + r8 - 16]   ; Load SSE register (16 bytes) from end of src.
  movups xmmword ptr [rax + r8 - 16], xmm0   ; Store into end of dst. 
  ret

_5_to_8:
  mov ecx, dword ptr [rdx]                   ; Load double-word (4 bytes) from src.
  mov dword ptr [rax], ecx                   ; Store in dst.
  mov ecx, dword ptr [rdx + r8 - 4]          ; Load double-word (4 bytes) from end of src.
  mov dword ptr [rax + r8 - 4], ecx          ; Store in end of dst.
  ; Falls into _return.

_return:
  ret

_above_64:
  cmp r8, 128                                ; if (size > 128) _above_128();
  ja _above_128

  ; 64-128
  vmovaps ymm0, ymmword ptr [rdx + r8 - 32]  ; Load AVX register from end of src (save for later).
  vmovaps ymm1, ymmword ptr [rdx]            ; Load AVX register from src.
  vmovaps ymm2, ymmword ptr [rdx + 32]       ; Load second AVX register from src.
  cmp r8, 96                                 ; if (size <= 96) _64_to_128();
  jbe _64_to_128
  
  ; 97-128
  vmovaps ymm3, ymmword ptr [rdx + 64]       ; Load AVX register extra 32 byte bridge.
  vmovaps ymmword ptr [rax + 64], ymm3       ; Store in dst.
  ; Falls into (64-128).
 
ALIGN 4
_64_to_128:
  vmovaps ymmword ptr [rax], ymm1            ; Store first AVX register into beginning of dst.
  vmovaps ymmword ptr [rax + 32], ymm2       ; Store next AVX register into beginning of dst.
  vmovaps ymmword ptr [rax + r8 - 32], ymm0  ; Store last AVX register into end of dst.
  vzeroupper                                 ; Clean up registers.
  ret
  
_above_128:
  lea rcx, [r8 - 128]                        ; Find the final 4-wide AVX register we can load (last_registers).
  xor r9, r9                                 ; Zero-out a counter for looping through 4-wide AVX load/stores (byte_counter).
  ; Falls into (>128) loop.

ALIGN 16
_above_128_loop:
  vmovdqa ymm0, ymmword ptr [rdx + r9]       ; Load/store 4 AVX registers at a time in loop.
  vmovdqa ymm1, ymmword ptr [rdx + r9 + 32]
  vmovdqa ymm2, ymmword ptr [rdx + r9 + 64]
  vmovdqa ymm3, ymmword ptr [rdx + r9 + 96]
  vmovdqa ymmword ptr [rax + r9], ymm0
  vmovdqa ymmword ptr [rax + r9 + 32], ymm1
  vmovdqa ymmword ptr [rax + r9 + 64], ymm2
  vmovdqa ymmword ptr [rax + r9 + 96], ymm3
  add r9, 128                                ; Increment byte counter.
  cmp r9, rcx                                ; if (byte_counter < last_registers) _above_128_loop();
  jb _above_128_loop
  ; Falls through to handle the tail bytes.

  lea rcx, [r8 - 32]                         ; Set the byte counter to the last AVX register (last_register).

  cmp rcx, r9                                ; if (last_register < byte_counter) _last_register();
  jb _last_register
  vmovdqu ymm0, ymmword ptr [rdx + r9]       ; Load an AVX register from src, at end of last loop.
  vmovdqu ymmword ptr [rax + r9], ymm0       ; Store in dst.
  add r9, 32                                 ; Iterate byte_counter to the next AVX register.

  cmp rcx, r9                                ; if (last_register < byte_counter) _last_register();
  jb _last_register
  vmovdqu ymm0, ymmword ptr [rdx + r9]       ; Load an AVX register from src, at end of last iteration.
  vmovdqu ymmword ptr [rax + r9], ymm0       ; Store in dst.
  add r9, 32                                 ; Iterate byte_counter to the next AVX register.
  
  cmp rcx, r9                                ; if (last_register < byte_counter) _last_register();
  jb _last_register
  vmovdqu ymm0, ymmword ptr [rdx + r9]       ; Load an AVX register from src, at end of last iteration.
  vmovdqu ymmword ptr [rax + r9], ymm0       ; Store in dst.
  ; Fall through to _last_register.

_last_register:
  vmovdqu ymm0, ymmword ptr [rdx + r8 - 32]  ; Load the AVX register at the very end of src.
  vmovdqu ymmword ptr [rax + r8 - 32], ymm0  ; Store in dst.
  vzeroupper                                 ; Clean registers.
  ret
                                    
asm_memcpy_aligned ENDP

;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;;

asm_memcpy_unaligned PROC

  ; rcx == dst
  ; rdx == src
  ; r8  == size
  ; rax == return

  mov rax, rcx                                 ; Move dst into rax register.

  cmp r8, 64                                   ; if (size > 64) _above_64();
  ja _above_64
  cmp r8, 16                                   ; if (size >= 16) _16_to_64();
  jae _16_to_64
  ; Falls into (0-15).

_0_to_15:
  cmp r8, 4                                    ; if (size <= 4) _0_to_4();
  jbe _0_to_4      
  cmp r8, 8                                    ; if (size <= 8) _5_to_8();
  jbe _5_to_8
  ; Falls into (9-15).

_9_to_15:
  mov rcx, qword ptr [rdx]                    ; Load quad-word (8 bytes) from src.
  mov qword ptr [rax], rcx                    ; Store into dst.
  mov rcx, qword ptr [rdx + r8 - 8]           ; Load quad-word (8 bytes) from end of src.
  mov qword ptr [rax + r8 - 8], rcx           ; Store into end of dst.
  ret

_0_to_4:
  cmp r8, 0                                   ; if (size == 0) return;
  je _return
  mov cl, byte ptr [rdx]                      ; Load 1 byte from src.
  mov byte ptr [rax], cl                      ; Store into dst.
  mov cl, byte ptr [rdx + r8 - 1]             ; Load 1 byte from end of src.
  mov byte ptr [rax + r8 - 1], cl             ; Store into end of dst.
  cmp r8, 2                                   ; if (size <= 2) return;
  jbe _return
  mov cl, byte ptr [rdx + 1]                  ; Load second byte from src.
  mov byte ptr [rax + 1], cl                  ; Store into dst.
  mov cl, byte ptr [rdx + 2]                  ; Load third byte from src.
  mov byte ptr [rax + 2], cl                  ; Store into dst.
  ret

_16_to_64:
  cmp r8, 32                                  ; if  (size <= 32) _16_to_32();
  jbe _16_to_32
  ; Falls into (32-64).

_32_to_64:
  vmovdqu ymm0, ymmword ptr [rdx]             ; Load AVX register from src.
  vmovdqu ymmword ptr [rax], ymm0             ; Store into dst.
  vmovdqu ymm0, ymmword ptr [rdx + r8 - 32]   ; Load AVX register from end of src.
  vmovdqu ymmword ptr [rax + r8 - 32], ymm0   ; Store into end of dst.
  vzeroupper                                  ; Clean up registers.
  ret

_16_to_32:
  movups xmm0, xmmword ptr [rdx]             ; Load SSE register (16 bytes) from src.
  movups xmmword ptr [rax], xmm0             ; Store into dst.
  movups xmm0, xmmword ptr [rdx + r8 - 16]   ; Load SSE register (16 bytes) from end of src.
  movups xmmword ptr [rax + r8 - 16], xmm0   ; Store into end of dst. 
  ret

_5_to_8:
  mov ecx, dword ptr [rdx]                   ; Load double-word (4 bytes) from src.
  mov dword ptr [rax], ecx                   ; Store in dst.
  mov ecx, dword ptr [rdx + r8 - 4]          ; Load double-word (4 bytes) from end of src.
  mov dword ptr [rax + r8 - 4], ecx          ; Store in end of dst.
  ; Falls into _return.

_return:
  ret

_above_64:
  cmp r8, 128                                ; if (size > 128) _above_128();
  ja _above_128

  ; 64-128
  vmovups ymm0, ymmword ptr [rdx + r8 - 32]  ; Load AVX register from end of src (save for later).
  vmovups ymm1, ymmword ptr [rdx]            ; Load AVX register from src.
  vmovups ymm2, ymmword ptr [rdx + 32]       ; Load second AVX register from src.
  cmp r8, 96                                 ; if (size <= 96) _64_to_128();
  jbe _64_to_128
  
  ; 97-128
  vmovups ymm3, ymmword ptr [rdx + 64]       ; Load AVX register extra 32 byte bridge.
  vmovups ymmword ptr [rax + 64], ymm3       ; Store in dst.
  ; Falls into (64-128).
 
ALIGN 4
_64_to_128:
  vmovups ymmword ptr [rax], ymm1            ; Store first AVX register into beginning of dst.
  vmovups ymmword ptr [rax + 32], ymm2       ; Store next AVX register into beginning of dst.
  vmovups ymmword ptr [rax + r8 - 32], ymm0  ; Store last AVX register into end of dst.
  vzeroupper                                 ; Clean up registers.
  ret
  
_above_128:
  lea rcx, [r8 - 128]                        ; Find the final 4-wide AVX register we can load (last_registers).
  xor r9, r9                                 ; Zero-out a counter for looping through 4-wide AVX load/stores (byte_counter).
  ; Falls into (>128) loop.

ALIGN 16
_above_128_loop:
  vmovdqu ymm0, ymmword ptr [rdx + r9]       ; Load/store 4 AVX registers at a time in loop.
  vmovdqu ymm1, ymmword ptr [rdx + r9 + 32]
  vmovdqu ymm2, ymmword ptr [rdx + r9 + 64]
  vmovdqu ymm3, ymmword ptr [rdx + r9 + 96]
  vmovdqu ymmword ptr [rax + r9], ymm0
  vmovdqu ymmword ptr [rax + r9 + 32], ymm1
  vmovdqu ymmword ptr [rax + r9 + 64], ymm2
  vmovdqu ymmword ptr [rax + r9 + 96], ymm3
  add r9, 128                                ; Increment byte counter.
  cmp r9, rcx                                ; if (byte_counter < last_registers) _above_128_loop();
  jb _above_128_loop
  ; Falls through to handle the tail bytes.

  lea rcx, [r8 - 32]                         ; Set the byte counter to the last AVX register (last_register).

  cmp rcx, r9                                ; if (last_register < byte_counter) _last_register();
  jb _last_register
  vmovdqu ymm0, ymmword ptr [rdx + r9]       ; Load an AVX register from src, at end of last loop.
  vmovdqu ymmword ptr [rax + r9], ymm0       ; Store in dst.
  add r9, 32                                 ; Iterate byte_counter to the next AVX register.

  cmp rcx, r9                                ; if (last_register < byte_counter) _last_register();
  jb _last_register
  vmovdqu ymm0, ymmword ptr [rdx + r9]       ; Load an AVX register from src, at end of last iteration.
  vmovdqu ymmword ptr [rax + r9], ymm0       ; Store in dst.
  add r9, 32                                 ; Iterate byte_counter to the next AVX register.
  
  cmp rcx, r9                                ; if (last_register < byte_counter) _last_register();
  jb _last_register
  vmovdqu ymm0, ymmword ptr [rdx + r9]       ; Load an AVX register from src, at end of last iteration.
  vmovdqu ymmword ptr [rax + r9], ymm0       ; Store in dst.
  ; Fall through to _last_register.

_last_register:
  vmovdqu ymm0, ymmword ptr [rdx + r8 - 32]  ; Load the AVX register at the very end of src.
  vmovdqu ymmword ptr [rax + r8 - 32], ymm0  ; Store in dst.
  vzeroupper                                 ; Clean registers.
  ret
                                    
asm_memcpy_unaligned ENDP
END