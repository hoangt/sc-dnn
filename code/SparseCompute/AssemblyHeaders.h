#ifdef LINUX_BUILD
#else // WINDOWS_BUILD
#include "stdafx.h"
#endif


extern "C"
{
    extern float mulsum2_baseline(const float *pf0, const float *pf1, INT64 count);
    extern float mulsum3_baseline(const float *pf0, const float *pf1, float f2, INT64 count);

    extern float mulsum2_unroll(const float *pf0, const float *pf1, INT64 count);
    extern float mulsum3_unroll(const float *pf0, const float *pf1, float f2, INT64 count);

    extern float mulsum2_sparse(const float *pf0, const float *pf1, INT64 count);
    extern float mulsum3_sparse(const float *pf0, const float *pf1, float f2, INT64 count);

    extern float mulsum2_hardware(const float* pf0, const float* pf1, INT64 denseCount);
    extern float mulsum3_hardware(const float *pf0, const float *pf1, float f2, INT64 count);
}
