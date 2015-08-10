.text

.globl mulsum2_opt1_75_25
mulsum2_opt1_75_25: 
	xorps	%xmm0,	%xmm0
	mov	%rdx,	%r9
	shr	$0x2,	%r9
	and	0xFFFFFFFFFFFFFFFC,	%r9 # $0x10 bytes alignment
	test	%r9,	%r9
	jz		loop_11_end
loop_11:
	movups	(%rdi), %xmm1
	movups	(%rsi), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_11_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_11_end
	movups	(%rdi), %xmm1
	movups	(%rsi), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_11_end
	movups	(%rdi), %xmm1
	movups	(%rsi), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	jne		loop_11
loop_11_end:
	haddps	%xmm0,	%xmm0
	haddps	%xmm0,	%xmm0
	retq 

.globl mulsum2_opt1_50_50
mulsum2_opt1_50_50:		
	xorps	%xmm0,	%xmm0
	mov	%rdx,	%r9
	shr	$0x2,	%r9
	and	0xFFFFFFFFFFFFFFFC,		%r9 # $0x10 bytes alignment
	test %r9,	%r9
	jz		loop_12_end
loop_12:
	movups	(%rdi), %xmm1
	movups	(%rsi), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_12_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_12_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_12_end
	movups	(%rdi), %xmm1
	movups	(%rsi), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	jne		loop_12
loop_12_end:
	haddps	%xmm0,	%xmm0
	haddps	%xmm0,	%xmm0
	retq 

.globl mulsum2_opt1_25_75 
mulsum2_opt1_25_75:
	xorps	%xmm0,	%xmm0
	mov	%rdx,	%r9
	shr	$0x2,	%r9
	and	0xFFFFFFFFFFFFFFFC,	%r9 # $0x10 bytes alignment
	test	%r9,	%r9
	jz		loop_13_end
loop_13:
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_13_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_13_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_13_end
	movups	(%rdi), %xmm1
	movups	(%rsi), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	jne		loop_13
loop_13_end:
	haddps	%xmm0,	%xmm0
	haddps	%xmm0,	%xmm0
	retq 


.globl mulsum2_opt1_0_100 
mulsum2_opt1_0_100:
	xorps	%xmm0,	%xmm0
	mov	%rdx,	%r9
	shr	$0x2,	%r9
	and	0xFFFFFFFFFFFFFFFC,	%r9 # $0x10 bytes alignment
	test	%r9,	%r9
	jz		loop_14_end
loop_14:
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_14_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_14_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	je		loop_14_end
	movups	(%rdi), %xmm1
	nop
	nop
	nop
	add	$0x10,	%rdi
	add	$0x10,	%rsi
	dec	%r9
	jne		loop_14
loop_14_end:
	haddps	%xmm0,	%xmm0
	haddps	%xmm0,	%xmm0
	retq 
