.code

; 25% sparsity in activation (rcx)
mulsum2_opt2_75_25 PROC
 	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		4  ; cache line count
	test	r9,		r9
	jz		loop_11_end
loop_11:
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_11_end
    mov r10, 3
loop_12:        
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
    jz loop_11_end
    dec             r10
    jne loop_12
    jmp loop_11
loop_11_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
    jz loop_13_end
loop_13:        
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
    dec r9
    jne loop_13
loop_13_end:    
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt2_75_25	ENDP

; 50% sparsity in activation (rcx)
mulsum2_opt2_50_50 PROC
 	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		4  ; cache line count
	test	r9,		r9
	jz		loop_21_end
loop_21:
        xorps xmm1, xmm1
        add rcx, 64
        add rdx, 64
        dec r9
        jz loop_21_end
        xorps xmm1, xmm1
        add rcx, 64
        add rdx, 64
        dec r9
        jz loop_21_end
        mov r10, 2
loop_22:        
 	movups	xmm1,	xmmword ptr [rcx]
 	movups	xmm2,	xmmword ptr [rdx]
 	mulps	xmm1,	xmm2
 	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
 	movups	xmm1,	xmmword ptr [rcx]
 	movups	xmm2,	xmmword ptr [rdx]
 	mulps	xmm1,	xmm2
 	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
 	movups	xmm1,	xmmword ptr [rcx]
 	movups	xmm2,	xmmword ptr [rdx]
 	mulps	xmm1,	xmm2
 	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
 	movups	xmm1,	xmmword ptr [rcx]
 	movups	xmm2,	xmmword ptr [rdx]
 	mulps	xmm1,	xmm2
 	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
    jz loop_21_end
    dec             r10
    jne loop_22
    jmp loop_21
loop_21_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
    jz loop_23_end
loop_23:        
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
    dec r9
    jne loop_23
loop_23_end:    
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt2_50_50	ENDP

; 75% sparsity in activation (rcx)
mulsum2_opt2_25_75 PROC
 	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		4  ; cache line count
	test	r9,		r9
	jz		loop_31_end
loop_31:
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_31_end
    mov r10, 1
loop_32:        
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
	dec		r9
    jz loop_31_end
    dec             r10
    jne loop_32
    jmp loop_31
loop_31_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
    jz loop_33_end
loop_33:        
	movups	xmm1,	xmmword ptr [rcx]
	movups	xmm2,	xmmword ptr [rdx]
	mulps	xmm1,	xmm2
	addps	xmm0,	xmm1
	add		rcx,	16
	add		rdx,	16
    dec r9
    jne loop_33
loop_33_end:    
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt2_25_75	ENDP

; 100% sparsity in activation (rcx)
mulsum2_opt2_0_100 PROC
 	xorps	xmm0,	xmm0
	mov		r9,		r8
	shr		r9,		4  ; cache line count
	test	r9,		r9
	jz		loop_41_end
loop_41:
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_41_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_41_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jz loop_41_end
    xorps xmm1, xmm1
    add rcx, 64
    add rdx, 64
    dec r9
    jne loop_41
loop_41_end:
    mov r9, r8
    and r9, 15
	shr r9, 2
    test r9, r9
    jz loop_43_end
loop_43:        
	add		rcx,	16
	add		rdx,	16
    dec r9
    jne loop_43
loop_43_end:    
	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt2_0_100	ENDP

mulsum2_opt2_nop PROC
	ret 0
mulsum2_opt2_nop	ENDP

END
