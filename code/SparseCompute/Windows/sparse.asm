.code

;
; extern float  mulsum_2(const float *pf0, const float *pf1, INT64 count);
;
;	for(INT64 i = 0; i < counut; i++);
;	{
;       sum += pf0[i] * pf1[i];
;   }
;   return sum;
;
mulsum2_sparse PROC
    xorps	xmm0,	xmm0
    xorps   xmm3,   xmm3
    mov		r9,		r8
    shr		r9,		2	
    test	r9,		r9
    jz		loop_1_end
loop_1:
    movups	xmm1,	xmmword ptr [rcx]
    ptest   xmm3,   xmm1
    jnz     sparse_1
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
sparse_1:
    add		rcx,	16
    add		rdx,	16
    dec		r9
    je		loop_1_end
    movups	xmm1,	xmmword ptr [rcx]
    ptest   xmm3,   xmm1
    jnz     sparse_2
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
sparse_2:
    add		rcx,	16
    add		rdx,	16
    dec		r9
    je		loop_1_end
    movups	xmm1,	xmmword ptr [rcx]
    ptest   xmm3,   xmm1
    jnz     sparse_3
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
sparse_3:
    add		rcx,	16
    add		rdx,	16
    dec		r9
    je		loop_1_end
    movups	xmm1,	xmmword ptr [rcx]
    ptest   xmm3,   xmm1
    jnz     sparse_4
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
sparse_4:
    add		rcx,	16
    add		rdx,	16
    dec		r9
    jne		loop_1
loop_1_end:
    haddps	xmm0,	xmm0
    haddps	xmm0,	xmm0
    ret 0
mulsum2_sparse	ENDP

mulsum3_sparse PROC
; extern void  mulsum3_sparse(const float *pf0, const float *pf1, float f2, INT64 count);
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
    xorps xmm0, xmm0
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
    ptest   xmm0,   xmm1
    jnz     sparse_31
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
sparse_31:
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_1_end
    movups xmm1, xmmword ptr [rdx]
    ptest   xmm0,   xmm1
    jnz     sparse_32
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
sparse_32:
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_1_end
    movups xmm1, xmmword ptr [rdx]
    ptest   xmm0,   xmm1
    jnz     sparse_33
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
sparse_33:
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_1_end
    movups xmm1, xmmword ptr [rdx]
    ptest   xmm0,   xmm1
    jnz     sparse_34
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
sparse_34:
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_1
loop_1_end:
    ret   0
mulsum3_sparse ENDP

END
