.text

# extern void  avx2_mulsum_3_mem(const float *pf0, const float *pf1, float f2, INT64 count);
#
#	for(INT64 i = 0; i < counut; i++);
#	{
#       pf0[i] += pf1[i] * f2;
#  }

; rcx pf0
; rdx pf1
; xmm2 f2
; r9  count

.globl mulsum3_base
mulsum3_base:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %r9, %r8
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_1_end
loop_1:
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_1_end
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_1_end
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    je   loop_1_end
	movups	(%rdx),	%xmm1
	movups	(%rcx), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rcx)
    add		$0x10,	%rcx
    add		$0x10,	%rdx
    dec		%r9
    jne   loop_1
loop_1_end:
    retq

