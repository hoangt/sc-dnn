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
mulsum2_base PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		4	; cache line count
	test	r9,		r9
	jz		loop_1_end
loop_1:
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
 	je		loop_1_end
 	movups	xmm1,	xmmword ptr [rcx]
 	movups	xmm2,	xmmword ptr [rdx]
 	mulps	xmm1,	xmm2
 	addps	xmm0,	xmm1
 	add		rcx,	16
 	add		rdx,	16
 	dec		r9
 	je		loop_1_end
 	movups	xmm1,	xmmword ptr [rcx]
 	movups	xmm2,	xmmword ptr [rdx]
 	mulps	xmm1,	xmm2
 	addps	xmm0,	xmm1
 	add		rcx,	16
 	add		rdx,	16
 	dec		r9
 	je		loop_1_end
 	movups	xmm1,	xmmword ptr [rcx]
 	movups	xmm2,	xmmword ptr [rdx]
 	mulps	xmm1,	xmm2
 	addps	xmm0,	xmm1
 	add		rcx,	16
 	add		rdx,	16
 	dec		r9
	jne		loop_1
loop_1_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
    jz loop_2_end
loop_2:
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
    dec r9
    jne loop_2	
loop_2_end:	
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_base	ENDP

END
