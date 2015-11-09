.text

#
# extern float  mulsum_2(const float *pf0, const float *pf1, INT64 count)#
#
#	for(INT64 i = 0# i < counut# i++)#
#	{
#       sum += pf0[i] * pf1[i]#
#   }
#   return sum#
#
.globl mulsum2_baseline
mulsum2_baseline:	
	xorps	%xmm0,	%xmm0
	mov	%rdx,	%r9
	shr	$0x2,	%r9
	and	$0xFFFFFFFFFFFFFFFC, %r9 # $0x10 bytes alignment
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


# extern void  mulsum3_baseline(const float *pf0, const float *pf1, float f2, INT64 count);
#
#	for(INT64 i = 0; i < counut; i++);
#	{
#       pf0[i] += pf1[i] * f2;
#  }

# rdi pf0
# rsi pf1
# xmm2 f2
# rdx  count

.globl mulsum3_baseline
mulsum3_baseline:
	movss %xmm2, -0x10(%rsp)
	movss %xmm2, -0xc(%rsp)
	movss %xmm2, -0x8(%rsp)
	movss %xmm2, -0x4(%rsp)
	movups -0x10(%rsp), %xmm2

    mov   %rdx, %r8
    shr   $0x2, %rdx
    test  %rdx,	%rdx
    jz    loop_1_end
loop_1:
	movups	(%rsi),	%xmm1
	movups	(%rdi), %xmm3
	mulps	%xmm2,	%xmm1
	addps	%xmm1,	%xmm3
	movups	%xmm3,	(%rdi)
    add		$0x10,	%rdi
    add		$0x10,	%rsi
    dec		%rdx
    jne   loop_1
loop_1_end:
    retq

