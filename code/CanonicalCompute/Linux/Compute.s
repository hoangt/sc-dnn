.text

#
# extern float  mulsum_2(const float *pf0, const float *pf1, INT64 count);
#
#	for(INT64 i = 0; i < counut; i++);
#	{
#       sum += pf0[i] * pf1[i];
#   }
#   return sum;
#
.globl avx2_mulsum_2_mem
avx2_mulsum_2_mem:
	xorps	%xmm0,	%xmm0
	mov	%r8,	%r9
	shr	$0x2,	%r9
	test	%r9,	%r9
	jz		loop_1_end
loop_1:
	movups	(%rcx), %xmm1
	movups	(%rdx), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	16,	%rcx
	add	16,	%rdx
	dec	%r9
	jne		loop_1
loop_1_end:
	haddps	%xmm0,	%xmm0
	haddps	%xmm0,	%xmm0
	retq 


# extern void  avx2_mulsum_3_mem(const float *pf0, const float *pf1, float f2, INT64 count);
#
#	for(INT64 i = 0; i < counut; i++);
#	{
#       pf0[i] += pf1[i] * f2;
#   }

# rcx pf0
# rdx pf1
# xmm2 f2
# r9  count

.globl avx2_mulsum_3_mem
avx2_mulsum_3_mem:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %r9, %r8
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_1_end
loop_12:
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

#
# extern float  fmemcpy(const float *pfSrc, float *pfDst, INT64 count);
#
# void fmemcpy(const float *pfSrc, float *pfDst, INT64 count);
# {
#     for (INT64 i = 0; i < length; i++)
#     {
#         *pfDst++ = *pfSrc++;
#     }
# }
# rcx pfSrc
# rdx pfDst
# r8 count

.globl avx2_fmemcpy
avx2_fmemcpy:
    mov   %r8,	%r9
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_1_end
loop_13:
	movups	(%rcx),	%xmm0
	movups	%xmm0, (%rdx)
    add   $0x10,	%rcx
    add   $0x10,	%rdx
    dec   %r9
    jne   loop_13
loop_13_end:
    and   $0x3,	%r8
    test  %r8,	%r8
    jz    loop_23_end
loop_23:
	movss (%rcx),	%xmm0
	movss %xmm0,	(%rdx)
    add   $0x4,		%rcx
    add   $0x4,		%rdx
    dec   %r8
    jne   loop_23
loop_23_end:
	retq

