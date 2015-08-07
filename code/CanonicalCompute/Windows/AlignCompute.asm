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
align_mulsum_2_mem PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
	test	r9,		r9
	jz		loop_1_end
loop_1:
	movaps	xmm1,	xmmword ptr [rcx]
	movaps	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
	jne		loop_1
loop_1_end:
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
align_mulsum_2_mem	ENDP


align_mulsum_3_mem PROC
; extern void  align_mulsum_3_mem(const float *pf0, const float *pf1, float f2, INT64 count);
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
    jne   loop_1
loop_1_end:
    ret   0
align_mulsum_3_mem ENDP

;
; extern float  fmemcpy(const float *pfSrc, float *pfDst, INT64 count);
;
; void fmemcpy(const float *pfSrc, float *pfDst, INT64 count);
; {
;     for (INT64 i = 0; i < length; i++)
;     {
;         *pfDst++ = *pfSrc++;
;     }
; }
align_fmemcpy PROC
; rcx pfSrc
; rdx pfDst
; r8 count
    mov   r9, r8
    shr   r9, 2
    test  r9, r9
    jz    loop_1_end
loop_1:
    movups xmm0, xmmword ptr [rcx]
    movups xmmword ptr [rdx], xmm0
    add   rcx, 16
    add   rdx, 16
    dec   r9
    jne   loop_1
loop_1_end:
    and   r8, 3
    test  r8, r8
    jz    loop_2_end
loop_2:
    movss xmm0, dword ptr [rcx]
    movss dword ptr [rdx], xmm0
    add   rcx, 4
    add   rdx, 4
    dec   r8
    jne   loop_2
loop_2_end:
    ret   0
align_fmemcpy ENDP


END
