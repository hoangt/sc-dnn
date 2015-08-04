.code

mulsum3_base PROC
; extern void  avx2_mulsum_3_mem(const float *pf0, const float *pf1, float f2, INT64 count);
;
;	for(INT64 i = 0; i < counut; i++);
;	{
;       pf0[i] += pf1[i] * f2;
;   }

; rcx pf0
; rdx pf1
; xmm2 f2
; r9  count
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 2
    test  r9, r9
    jz    loop_1_end
loop_1:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_1_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_1_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_1_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_1
loop_1_end:
    ret   0
mulsum3_base ENDP



END
