.code
; extern void  mulsum_3(const float *pf0, const float *pf1, float f2, INT64 denseCount, INT64 sparseCount);
;
;	for(INT64 i = 0; i < denseCounut; i++);
;	{
;       pf0[i] += pf1[i] * f2;
;   }

; rcx pf0
; rdx pf1
; xmm2 f2
; r9  denseCount
; rax sparseCount

; sparsity in activation(rdx)
mulsum3_opt3_DenseSparse PROC 
    xorps xmm3, xmm3

    movss dword ptr[rsp-10h], xmm2
    movss dword ptr[rsp-0ch], xmm2
    movss dword ptr[rsp-08h], xmm2
    movss dword ptr[rsp-04h], xmm2
    movups xmm2, dword ptr[rsp-10h]

	mov	  r10, rax
    mov   r8, r9
    shr   r9, 4                 ; cache line size
    test  r9, r9
	jz	  loop_11_end
loop_11:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_11_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_11_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    je   loop_11_end
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_11
loop_11_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
	jz loop_12_end
loop_12:
    movups xmm1, xmmword ptr [rdx]
    movups xmm3, xmmword ptr [rcx]
    mulps xmm1, xmm2
    addps xmm3, xmm1
    movups xmmword ptr [rcx], xmm3
    add   rcx, 16
    add   rdx, 16
    dec   r9
	jne	  loop_12
loop_12_end:
; End Dense Worker
; Start Sparse Work: Handle liveouts since all intermediate values are zeroes
	shl		rax,	2; multiply sparseCount by 4
	add		rcx,	rax
	movups  xmm3,	xmmword ptr -16[rcx]	; load last delta weight values
	add		rdx,	rax
	xorps	xmm1,	xmm1
	xor		rax,	rax
ret 0
mulsum3_opt3_DenseSparse ENDP

;	Mimic loop exit compensation code for liveouts
mulsum3_opt3_zerosigw PROC
    shl		r9,		2			; multiply count by 4
	add		rdx,	r9
	add		rcx,	r9

	movups	xmm3,	xmmword ptr -4[rcx]
	xorps	xmm1,	xmm1
	xor		r9,		r9
	ret 0
mulsum3_opt3_zerosigw ENDP

END
