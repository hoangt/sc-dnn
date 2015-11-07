.code

;
; extern float  mulsum2_hardware(const float *pf0, const float *pf1, INT64 denseCount);
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
mulsum2_hardware PROC
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
;   ret 0
; End Dense work
; Start Sparse work - Handle liveouts since only zero intermediate values are generated
;    shl		r9,		2	; multiply by 4
;    add		rdx,	r9
;    movups	xmm2,	xmmword ptr -16[rdx] ; load last weight values
;    add		rcx,	r9
;    xorps	xmm1,	xmm1	; since last activation value is zero
;    xor		r9,		r9		; since r9 is zero on exit
    haddps	xmm0,	xmm0
    haddps	xmm0,	xmm0
    ret 0
mulsum2_hardware ENDP

;	Mimic loop exit compensation code for liveouts
mulsum3_hardware PROC
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
    shl		r9,		2			; multiply count by 4
    add		rdx,	r9
    add		rcx,	r9

    movups	xmm3,	xmmword ptr -4[rcx]
    xorps	xmm1,	xmm1
    xor		r9,		r9
    ret 0
mulsum3_hardware ENDP

END
