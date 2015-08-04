.code

mulsum2_opt1_75_25 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
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
	nop
	nop
	nop
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
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_75_25	ENDP

mulsum2_opt1_50_50 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
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
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_1_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
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
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_50_50	ENDP

mulsum2_opt1_25_75 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
	test	r9,		r9
	jz		loop_1_end
loop_1:
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_1_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_1_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
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
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_25_75	ENDP

mulsum2_opt1_0_100 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
	test	r9,		r9
	jz		loop_1_end
loop_1:
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_1_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_1_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_1_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	jne		loop_1
loop_1_end:
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_0_100	ENDP

END
