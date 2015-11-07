.code

;
; extern float  mulsum_2(const float *pf0, const float *pf1, INT64 denseCount, INT64 sparseCount);
;
;	for(INT64 i = 0; i < denseCount; i++);
;	{
;       sum += pf0[i] * pf1[i];
;   }
;   return sum;

;  rcx pf0
;  rdx pf1
;  r8  denseCount
;  r9  sparseCount
; sparsity in activation (rcx)
mulsum2_opt3_DenseSparse PROC
    xorps   xmm0,  xmm0
    mov     r10,   r8
    shr		r10,	4	; cache line count
    test	r10,	r10
    jz		loop_11_end
;   Do dense work first
loop_11:
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r10
    je		loop_11_end
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r10
    je		loop_11_end
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r10
    je		loop_11_end
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r10
    jne		loop_11
loop_11_end:
    mov r10, r8
    and r10, 15
    shr r10, 2
    test r10, r10
    jz loop_12_end
loop_12:
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec r10
    jne loop_12
loop_12_end:	
; End Dense work
; Start Sparse work - Handle liveouts since only zero intermediate values are generated
    shl		r9,		2	; multiply by 4
    add		rdx,	r9
    movups	xmm2,	xmmword ptr -16[rdx] ; load last weight values
    add		rcx,	r9
    xorps	xmm1,	xmm1	; since last activation value is zero
    xor		r9,		r9		; since r9 is zero on exit

    haddps	xmm0,	xmm0
    haddps	xmm0,	xmm0
    ret 0
mulsum2_opt3_DenseSparse ENDP


END
