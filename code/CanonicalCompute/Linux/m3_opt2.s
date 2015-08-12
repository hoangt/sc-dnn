.text

.globl mulsum3_opt2_75_25 
mulsum3_opt2_75_25:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

	mov %rdx, %r8
	shr $0x4, %rdx # cache line count
	test %rdx, %rdx
	jz loop_11_end
loop_11:
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_11_end
	mov $0x3, %r10
loop_12:
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	dec %rdx
	jz loop_11_end
	dec %r10
	jne loop_12
	jmp loop_11
loop_11_end:
	mov %r8, %rdx
	and $0xF, %rdx
        shr $0x2, %rdx
	test %rdx, %rdx
	jz loop_13_end
loop_13:  
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	dec %rdx
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

	mov %rdx, %r8
	shr $0x4, %rdx # cache line count
	test %rdx, %rdx
	jz loop_21_end
loop_21:
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_21_end
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_21_end
	mov $0x2, %r10
loop_22:
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	dec %rdx
	jz loop_21_end
	dec %r10
	jne loop_22
	jmp loop_21
loop_21_end:
	mov %r8, %rdx
	and $0xF, %rdx
        shr $0x2, %rdx
	test %rdx, %rdx
	jz loop_23_end
loop_23:  
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	dec %rdx
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
        
	mov %rdx, %r8
	shr $0x4, %rdx # cache line count
	test %rdx, %rdx
	jz loop_31_end
loop_31:
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_31_end
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_31_end
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_31_end
	mov $0x1, %r10
loop_32:
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	dec %rdx
	jz loop_31_end
	dec %r10
	jne loop_32
	jmp loop_31
loop_31_end:
	mov %r8, %rdx
	and $0xF, %rdx
        shr $0x2, %rdx
	test %rdx, %rdx
	jz loop_33_end
loop_33:  
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	dec %rdx
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
        
	mov %rdx, %r8
	shr $0x4, %rdx
	test %rdx, %rdx
	jz loop_41_end
loop_41:
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_41_end
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_41_end
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jz loop_41_end
	xorps %xmm1, %xmm1
	add $0x40, %rdi
	add $0x40, %rsi
	dec %rdx
	jne loop_41
loop_41_end:
	mov %r8, %rdx
	and $0xF, %rdx
        shr $0x2, %rdx
	test %rdx, %rdx
	jz loop_43_end
loop_43:          
	movups (%rsi), %xmm1
	movups (%rdi), %xmm3
	mulps %xmm2, %xmm1
	addps %xmm1, %xmm3
	movups %xmm3, (%rdi)
	add $0x10, %rdi
	add $0x10, %rsi
	dec %rdx
	jne loop_43
loop_43_end:    
	retq    
        

	.globl mulsum3_opt2_zerosigw
mulsum3_opt2_zerosigw:
        shl $0x2, %rdx
        add %rdx, %rsi
        add %rdx, %rdi
        xorps %xmm2, %xmm2
        movups -0x4(%rdi), %xmm3
        xor %rdx, %rdx
        retq
        
