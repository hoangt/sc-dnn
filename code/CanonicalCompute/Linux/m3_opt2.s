.text

.globl mulsum3_opt2_75_25 
mulsum3_opt2_75_25:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

	mov	%r8,	%r9
	shr	$0x4,	%r9 ; cache line count
	test	%r9,	%r9
    jz    loop_11_end
loop_11:
    xorps	%xmm1,	%xmm1
    add		64,		%rcx
    add		64,		%rdx
    dec		%r9
    jz loop_11_end
    mov		3,		%r10
loop_12:
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    jz loop_11_end
    dec		%r10
    jne loop_12
    jmp loop_11
loop_11_end:
     mov	%r8, %r9
     and	15,	 %r9
     test	%r9, %r9
    jz loop_13_end
loop_13:        
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    jne loop_13
loop_13_end:    
    retq

.globl mulsum3_opt2_50_50 
mulsum3_opt2_50_50:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

	mov	%r8,	%r9
	shr	$0x4,	%r9 ; cache line count
	test	%r9,	%r9
    jz    loop_21_end
loop_21:
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_21_end
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_21_end
    mov		2,		%r10
loop_22:
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    jz loop_21_end
    dec		%r10
    jne loop_22
    jmp loop_21
loop_21_end:
    mov	%r8, %r9
    and	15,	 %r9
    test	%r9, %r9
    jz loop_23_end
loop_23:        
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    jne loop_23
loop_23_end:    
    retq

.globl mulsum3_opt2_25_75 
mulsum3_opt2_25_75:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

	mov	%r8,	%r9
	shr	$0x4,	%r9 ; cache line count
	test	%r9,	%r9
    jz    loop_31_end
loop_31:
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_31_end
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_31_end
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_31_end
    mov		1,		%r10
loop_32:
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec	   %r9
    jz loop_31_end
    dec	   %r10
    jne loop_32
    jmp loop_31
loop_31_end:
    mov	%r8, %r9
    and	15,	 %r9
    test	%r9, %r9
    jz loop_33_end
loop_33:        
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec	   %r9
    jne loop_33
loop_33_end:    
    retq

.globl mulsum3_opt2_0_100 
mulsum3_opt2_0_100:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

	mov	%r8,	%r9
	shr	$0x4,	%r9 ; cache line count
	test	%r9,	%r9
    jz    loop_31_end
loop_31:
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_31_end
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_31_end
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jz loop_31_end
	xorps	%xmm1,	%xmm1
	add		64,		%rcx
	add		64,		%rdx
	dec		%r9
    jne loop_31
loop_31_end:
    mov	%r8, %r9
    and	15,	 %r9
    test	%r9, %r9
    jz loop_33_end
loop_33:        
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    jne loop_33
loop_33_end:    
    retq
