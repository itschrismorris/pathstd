.code
avx_memcpy_asm PROC

  ; rcx == dst
  ; rdx == src
  ; r8  == size
  ; rax == return

  mov rax, rcx                                 ; Move dst into rax register.
  
  cmp r8, 64                                   ; if (size > 64) _above_64()
  ja _above_64
  cmp r8, 16                                   ; if (size >= 16) _16_to_64()
  jae _16_to_64
  ; Falls into (0-15)

_0_to_15:
  cmp r8, 4                                    ; if (size <= 4) _0_to_4()
  jbe _0_to_4      
  cmp r8, 8                                    ; if (size <= 8) _4_to_8()
  jbe _5_to_8
  ; Falls into (9-15)

_9_to_15:
  mov rcx, qword ptr [rdx]                    ; Load quad-word (8 bytes) from src.
  mov qword ptr [rax], rcx                    ; Store into dst.
  mov rcx, qword ptr [rdx + r8 - 8]           ; Load quad-word (8 bytes) from end of src (maybe overlap).
  mov qword ptr [rax + r8 - 8], rcx           ; Store into end of dst.
  ret

_16_to_64:
  cmp r8, 32                                  ; if  (size <= 32) _16_to_32()
  jbe _16_to_32
  ; Falls into (32-64)

_32_to_64:
  ret

_0_to_4:
  ret

_5_to_8:
  ret

_16_to_32:
  movups xmm0, xmmword ptr [rdx]             ; Load SSE register (16 bytes) from src.
  movups xmmword ptr [rax], xmm0             ; Store into dst.
  movups xmm0, xmmword ptr [rdx + r8 - 16]   ; Load SSE register (16 bytes) from end of src (maybe overlap).
  movups xmmword ptr [rax + r8 - 16], xmm0   ; Store into end of dst. 
  ret

_above_64:
  ret

done:
    ret             ; return dest (already in rax)
avx_memcpy_asm ENDP
END