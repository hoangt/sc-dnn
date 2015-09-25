.code
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

; 25% sparsity in activation (rdx)
mulsum3_opt2_75_25 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 4                 ; cache line size
    test  r9, r9
    jz    loop_11_end
loop_11:
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_11_end
    mov r10, 3
loop_12:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jz loop_11_end
    dec r10
    jne loop_12
    jmp loop_11
loop_11_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
    jz loop_13_end
loop_13:        
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne loop_13
loop_13_end:    
    ret   0
mulsum3_opt2_75_25 ENDP

; 50% sparsity in activation (rdx)
mulsum3_opt2_50_50 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 4                 ; cache line size
    test  r9, r9
    jz    loop_21_end
loop_21:
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_21_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_21_end
    mov r10, 2
loop_22:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jz loop_21_end
    dec r10
    jne loop_22
    jmp loop_21
loop_21_end:
    mov r9, r8
    and r9, 15
	shr r9,	2
    test r9, r9
    jz loop_23_end
loop_23:        
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne loop_23
loop_23_end:    
    ret   0
mulsum3_opt2_50_50 ENDP

; 75% sparsity in activation (rdx)
mulsum3_opt2_25_75 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 4                 ; cache line size
    test  r9, r9
    jz    loop_31_end
loop_31:
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    mov r10, 1
loop_32:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jz loop_31_end
    dec r10
    jne loop_32
    jmp loop_31
loop_31_end:
    mov r9, r8
    and r9, 15
	shr	r9, 2
    test r9, r9
    jz loop_33_end
loop_33:        
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne loop_33
loop_33_end:    
    ret   0
mulsum3_opt2_25_75 ENDP

; 100% sparsity in activation (rdx)
mulsum3_opt2_0_100 PROC
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 4                 ; cache line size
    test  r9, r9
    jz    loop_31_end
loop_31:
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jne loop_31
loop_31_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
    jz loop_33_end
loop_33:        
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne loop_33
loop_33_end:    
    ret   0
mulsum3_opt2_0_100 ENDP

END
