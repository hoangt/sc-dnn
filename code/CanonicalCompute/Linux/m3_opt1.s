.text

.globl mulsum3_opt1_75_25
mulsum3_opt1_75_25:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %r9, %r8
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_11_end
loop_11:
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_11_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_11_end
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_11_end
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
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

    mov   %r9, %r8
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_12_end
loop_12:
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_12_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_12_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_12_end
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
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

    mov   %r9, %r8
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_13_end
loop_13:
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_13_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_13_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_13_end
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
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

    mov   %r9, %r8
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_14_end
loop_14:
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_14_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_14_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_14_end
	movups	(%rdx),	%xmm1
    nop
    nop
    nop
    nop
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    jne   loop_14
loop_14_end:
    retq
