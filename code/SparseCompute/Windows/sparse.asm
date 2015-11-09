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
    shr		r9,		4	; cache line count
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

END
