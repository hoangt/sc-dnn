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
; sparsity in activation (rcx)
mulsum2_hardware PROC
    xorps   xmm0,  xmm0
    mov     r9,   r8
    shr		r9,	2
    test	r9,	r9
    jz		loop_11_end
;   Do dense work first
loop_11:
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r9
    je		loop_11_end
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r9
    je		loop_11_end
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r9
    je		loop_11_end
    movups	xmm1,	xmmword ptr [rcx]
    movups	xmm2,	xmmword ptr [rdx]
    mulps	xmm1,	xmm2
    addps	xmm0,	xmm1
    add		rcx,	16
    add		rdx,	16
    dec		r9
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
; extern void  mulsum_3(const float *pf0, const float *pf1, float f2, INT64 count);
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


weightupdate_hardware   PROC
; extern void  mulsum3_unroll(const float *pf0, const float *pf1, const float f2, INT64 denseCount, INT64 sparseCount);
;
;	for(INT64 i = 0; i < denseCount; i++);
;	{
;       pf0[i] += pf1[i] * f2;
;   }
;   for (INT64 j = 0; j < sparseCount; j++)
;   {
;       = pf1[i]; // Load pf1
;   } 

; rcx pf0
; rdx pf1
; xmm2 f2
; r9  denseCount
; r8 sparseCount
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

    mov   r8, r9
    shr   r9, 2
    test  r9, r9
    jz    loop_31_end
; Do dense portion first
loop_31:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_31_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_31_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_31_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_31
loop_31_end:
; Start sparse portion
    shr   r8, 2
    test  r8, 8
    jz    loop_32_end
loop_32:
    movups xmm1, xmmword ptr [rdx]
    add   rcx, 16
    add   rdx, 16
    dec   r8
    je   loop_32_end
    movups xmm1, xmmword ptr [rdx]
    add   rcx, 16
    add   rdx, 16
    dec   r8
    je   loop_32_end
    movups xmm1, xmmword ptr [rdx]
    add   rcx, 16
    add   rdx, 16
    dec   r8
    je   loop_32_end
    movups xmm1, xmmword ptr [rdx]
    add   rcx, 16
    add   rdx, 16
    dec   r8
    jne   loop_32
loop_32_end:

    ret 0
weightupdate_hardware   ENDP

END
