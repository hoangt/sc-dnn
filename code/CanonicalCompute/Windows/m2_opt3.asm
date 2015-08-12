.code

;
; extern float  mulsum_2(const float *pf0, const float *pf1, INT64 denseCount, INT64 sparseCount);
;
;	for(INT64 i = 0; i < denseCount; i++);
;	{
;       sum += pf0[i] * pf1[i];
;   }
;   return sum;

;  rcx pf0
;  rdx pf1
;  r8  denseCount
;  r9  sparseCount
mulsum2_opt3_DenseSparse PROC
 	xorps	xmm0,	xmm0

	haddps	xmm0,	xmm0
	haddps	xmm0,	xmm0
	ret 0
mulsum2_opt3_DenseSparse ENDP


END
