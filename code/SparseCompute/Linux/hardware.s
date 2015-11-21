.text

#
# extern float  mulsum_2(const float *pf0, const float *pf1, INT64 denseCount)
#
#	for(INT64 i = 0# i < counut# i++)#
#	{
#       sum += pf0[i] * pf1[i]#
#   }
#   return sum#
#
.globl mulsum2_hardware
mulsum2_hardware:	
    xorps	%xmm0,	%xmm0
    mov	%rdx,	%r9
    shr	$0x2,	%r9
#	and	$0xFFFFFFFFFFFFFFFC, %r9 # $0x10 bytes alignment
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
    je		loop_1_end
    movups	(%rdi), %xmm1
    movups	(%rsi), %xmm2
    mulps	%xmm2,	%xmm1
    addps	%xmm1,	%xmm0
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%r9
    je		loop_1_end
    movups	(%rdi), %xmm1
    movups	(%rsi), %xmm2
    mulps	%xmm2,	%xmm1
    addps	%xmm1,	%xmm0
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%r9
    je		loop_1_end
    movups	(%rdi), %xmm1
    movups	(%rsi), %xmm2
    mulps	%xmm2,	%xmm1
    addps	%xmm1,	%xmm0
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%r9
    jne		loop_1
loop_1_end:
# End Dense work
# Start Sparse work - Handle liveouts since only zero intermediate values are generated
#	shl		0x2,	r9
#   add		%r9,	%rdx
#   movups	-0x10(%rdx),	%xmm2
#   add		%r9,	%rcx
#   xorps	%xmm1,	%xmm1
#   xor		%r9,	%r9
    haddps	%xmm0,	%xmm0
    haddps	%xmm0,	%xmm0
    retq 
    
    
# extern void  mulsum3(const float *pf0, const float *pf1, float f2, INT64 count)

#	for(INT64 i = 0; i < counut; i++);
#	{
#       pf0[i] += pf1[i] * f2;
#  }

# rdi pf0
# rsi pf1
# xmm2 f2
# rdx  count
# Simulate loop exit compensation code for liveouts
.globl mulsum3_hardware
mulsum3_hardware:
    shl	$0x2,	%rdx
    add	%rdx,	%rdi
    add	%rdx,	%rsi
    
    movups	-0x4(%rsi), %xmm3
    xorps	%xmm1,	%xmm1
    xor	%rdx,	%rdx
    retq

# extern void  mulsum3_unroll(const float *pf0, const float *pf1, const float f2, INT64 denseCount, INT64 sparseCount);
#
#	for(INT64 i = 0; i < denseCount; i++);
#	{
#       pf0[i] += pf1[i] * f2;
#   }
#   for (INT64 j = 0; j < sparseCount; j++)
#   {
#       = pf1[i]; // Load pf1
#   } 
# rdi pf0
# rsi pf1
# xmm2 f2
# rdx  denseCount
# rcx  sparseCount

.globl weightupdate_hadrware
weightupdate_hardware:
    movss %xmm2, -0x10(%rsp)
    movss %xmm2, -0xc(%rsp)
    movss %xmm2, -0x8(%rsp)
    movss %xmm2, -0x4(%rsp)
    movups -0x10(%rsp), %xmm2

    mov   %rdx, %r8
    shr   $0x2, %rdx
    test  %rdx,	%rdx
    jz    loop_31_end
loop_31:
    movups	(%rsi),	%xmm1
    movups	(%rdi), %xmm3
    mulps	%xmm2,	%xmm1
    addps	%xmm1,	%xmm3
    movups	%xmm3,	(%rdi)
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rdx
    je   loop_31_end
    movups	(%rsi),	%xmm1
    movups	(%rdi), %xmm3
    mulps	%xmm2,	%xmm1
    addps	%xmm1,	%xmm3
    movups	%xmm3,	(%rdi)
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rdx
    je   loop_31_end
    movups	(%rsi),	%xmm1
    movups	(%rdi), %xmm3
    mulps	%xmm2,	%xmm1
    addps	%xmm1,	%xmm3
    movups	%xmm3,	(%rdi)
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rdx
    je   loop_31_end
    movups	(%rsi),	%xmm1
    movups	(%rdi), %xmm3
    mulps	%xmm2,	%xmm1
    addps	%xmm1,	%xmm3
    movups	%xmm3,	(%rdi)
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rdx
    jne   loop_31
loop_31_end:
# Do sparse work
    shr   $0x2, %rcx
    test  %rdx,	%rcx
    jz    loop_32_end
loop_32:
    movups	(%rsi),	%xmm1
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rcx
    je   loop_32_end
    movups	(%rsi),	%xmm1
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rcx
    je   loop_32_end
    movups	(%rsi),	%xmm1
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rcx
    je   loop_32_end
    movups	(%rsi),	%xmm1
    add	$0x10,	%rdi
    add	$0x10,	%rsi
    dec	%rcx
    jne   loop_32
loop_32_end:
retq