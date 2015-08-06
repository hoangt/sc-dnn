.code

mulsum2_opt1_75_25 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
	and		r9,		-4 ; 16 bytes alignment
	test	r9,		r9
	jz		loop_11_end
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
	nop
	nop
	nop
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
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_75_25	ENDP

mulsum2_opt1_50_50 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
	and		r9,		-4 ; 16 bytes alignment
	test	r9,		r9
	jz		loop_12_end
loop_12:
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_12_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_12_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_12_end
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
	jne		loop_12
loop_12_end:
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_50_50	ENDP

mulsum2_opt1_25_75 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
	and		r9,		-4 ; 16 bytes alignment
	test	r9,		r9
	jz		loop_13_end
loop_13:
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_13_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_13_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_13_end
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
	jne		loop_13
loop_13_end:
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_25_75	ENDP

mulsum2_opt1_0_100 PROC
	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		2
	and		r9,		-4 ; 16 bytes alignment
	test	r9,		r9
	jz		loop_14_end
loop_14:
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_14_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_14_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	je		loop_14_end
	movups	xmm1,	xmmword ptr [rcx]
	nop
	nop
	nop
	add		rcx,	16
	add		rdx,	16
	dec		r9
	jne		loop_14
loop_14_end:
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt1_0_100	ENDP

END
