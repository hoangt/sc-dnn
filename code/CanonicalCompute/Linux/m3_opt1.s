.text

.globl mulsum3_opt1_75_25
mulsum3_opt1_75_25:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %rdx, %r8
    shr   $0x2, %rdx
    test  %rdx,	%rdx
    jz    loop_11_end
loop_11:
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_11_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_11_end
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_11_end
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    jne   loop_11
loop_11_end:
    retq

.globl mulsum3_opt1_50_50
mulsum3_opt1_50_50:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %rdx, %r8
    shr   $0x2, %rdx
    test  %rdx,	%rdx
    jz    loop_12_end
loop_12:
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_12_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_12_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_12_end
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    jne   loop_12
loop_12_end:
    retq


.globl mulsum3_opt1_25_75
mulsum3_opt1_25_75:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %rdx, %r8
    shr   $0x2, %rdx
    test  %rdx,	%rdx
    jz    loop_13_end
loop_13:
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_13_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_13_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_13_end
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    jne   loop_13
loop_13_end:
    retq


.globl mulsum3_opt1_0_100 
mulsum3_opt1_0_100:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %rdx, %r8
    shr   $0x2, %rdx
    test  %rdx,	%rdx
    jz    loop_14_end
loop_14:
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_14_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_14_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    je   loop_14_end
	movups	(%rsi),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    jne   loop_14
loop_14_end:
    retq
