.data
  message db 'Hello, World!', 0

.code
asm_memcpy PROC

  ; rcx == dst
  ; rdx == src
  ; r8  == size
  ; rax == return

  mov rax, rcx                                 ; Move dst into rax register.
  
  lea rdi, [rel message]                       ; Call printf so we can debug this.
  call printf


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
  mov rcx, qword ptr [rdx + r8 - 8]           ; Load quad-word (8 bytes) from end of src (maybe overlap).
  mov qword ptr [rax + r8 - 8], rcx           ; Store into end of dst.
  ret

_0_to_4:
  cmp r8, 0                                   ; if (size == 0) return;
  je _return
  mov cl, byte ptr [rdx]                      ; Load 1 byte from src.
  mov byte ptr [rax], cl                      ; Store into dst.
  mov cl, byte ptr [rdx + r8 - 1]             ; Load 1 byte from end of src (maybe overlap).
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
  vmovdqu ymm0, ymmword ptr [rdx]             ; Load AVX register (32 bytes) from src.
  vmovdqu ymmword ptr [rax], ymm0             ; Store into dst.
  vmovdqu ymm0, ymmword ptr [rdx + r8 - 32]   ; Load AVX register (32 bytes) from end of src (maybe overlap).
  vmovdqu ymmword ptr [rax + r8 - 32], ymm0   ; Store into end of dst.
  vzeroupper                                  ; Clean up registers.
  ret

_16_to_32:
  movups xmm0, xmmword ptr [rdx]             ; Load SSE register (16 bytes) from src.
  movups xmmword ptr [rax], xmm0             ; Store into dst.
  movups xmm0, xmmword ptr [rdx + r8 - 16]   ; Load SSE register (16 bytes) from end of src (maybe overlap).
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
  vmovups ymm0, ymmword ptr [rdx + r8 - 32]  ; Load AVX register (32 bytes) from end of src (save for later).
  vmovups ymm1, ymmword ptr [rdx]            ; Load AVX register (32 bytes) from src.
  vmovups ymm2, ymmword ptr [rdx + 32]       ; Load second AVX register (32 bytes) from src.
  cmp r8, 96                                 ; if (size <= 96) _64_to_128();
  jbe _64_to_128
  
  ; 97-128
  vmovups ymm3, ymmword ptr [rdx + r8 - 64]  ; Load AVX register (32 bytes) from
  vmovups ymmword ptr [rax + 64], ymm3       ; Store in dst.
  ret
  
_above_128:
_64_to_128:

                                    
asm_memcpy ENDP
END