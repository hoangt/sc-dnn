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
# %rdi = pf0
# %rsi = pf1
# %rdx = count
.globl avx2_mulsum_2_mem
avx2_mulsum_2_mem:
	xorps	%xmm0,	%xmm0
	mov	%rdx,	%r9
	shr	$0x2,	%r9
	test	%r9,	%r9
	jz		loop_1_end
loop_1:
	movups	(%rdi), %xmm1
	movups	(%rsi), %xmm2
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm0
	add	$0x10,	%rdi
	add	$0x10,	%rsi
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

# rdi pf0
# rsi pf1
# xmm0 f2
# rdx count

.globl avx2_mulsum_3_mem
avx2_mulsum_3_mem:
	movss %xmm0, -0x10(%rsp)
	movss %xmm0, -0xc(%rsp)
	movss %xmm0, -0x8(%rsp)
	movss %xmm0, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

	mov   %rdx, %r9
	shr   $0x2, %r9
	test  %r9,  %r9
	jz    loop_1_end
loop_12:
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
        add	$0x10,	%rdi
	add	$0x10,	%rsi
        dec	%r9
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
# rdi pfSrc
# rsi pfDst
# rdx count

.globl avx2_fmemcpy
avx2_fmemcpy:
    mov   %rdx,	%r9
    shr   $0x2, %r9
    test  %r9,	%r9
    jz    loop_1_end
loop_13:
    movups  (%rdi),	%xmm0
    movups  %xmm0,	(%rsi)
    add	    $0x10,	%rdi
    add     $0x10,	%rsi
    dec   %r9
    jne   loop_13
loop_13_end:
    and   $0x3,	%rdx
    test  %rdx,	%rdx
    jz    loop_23_end
loop_23:
    movss  (%rdi),	%xmm0
    movss  %xmm0,	(%rsi)
    add    $0x4,	%rdi
    add    $0x4,	%rsi
    dec    %rdx
    jne   loop_23
loop_23_end:
	retq

