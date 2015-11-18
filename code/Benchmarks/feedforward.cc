#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include "utils.h"
#include "xmmintrin.h"
#include "smmintrin.h"

using namespace std;

#ifdef SIMD_OPT
void feedforward (float *output, const float* input, const float* weights, int inputCount, int outputCount)
{
  for (int i = 0; i < outputCount; i++) {
    const float *tempWeights = weights + (i * inputCount);
    __m128 Z = _mm_setzero_ps();
    for (int j = 0; j < inputCount; j+=4) {
      __m128 X = _mm_load_ps(input+j);
#ifdef ZERO_OPT 
      if (!_mm_testz_si128((__m128i)X, (__m128i)X)) 
#endif
      {
	__m128 Y = _mm_load_ps(tempWeights+j);
	Z = _mm_add_ps(Z, _mm_mul_ps(X, Y));
      }
    }
    output[i] = Z[0];
  }
}
/*
#elif SIMD_OPT
void feedforward (float *output, const float* input, const float* weights, int inputCount, int outputCount)
{
  for (int i = 0; i < outputCount; i++) {
    const float *tempWeights = weights + (i * inputCount);
    __m128 Z = _mm_setzero_ps();
    for (int j = 0; j < inputCount; j+=4) {
      __m128 X = _mm_load_ps(input+j);
      __m128 Y = _mm_load_ps(tempWeights+j);
      Z = _mm_add_ps(Z, _mm_mul_ps(X, Y));
    }
    output[i] = Z[0];
  }
}
*/
#else
void feedforward (float *output, const float* input, const float* weights, int inputCount, int outputCount)
{
  for (int i = 0; i < outputCount; i++) {
    const float *tempWeights = weights + (i * inputCount);
    float sum = 0.0f;
    for (int j = 0; j < inputCount; j++) {      
#ifdef ZERO_OPT
      if (input[j] != 0)
#endif
	{
	  sum += input[j] * tempWeights[j];      
	}
    }
    output[i] = sum;
  }
}
#endif

int main (int argc, char** argv)
{
  if (argc < 5) {
    printf ("Usage: %s <InputCount> <OutputCount> <SparseRatio> <Iteration>\n", argv[0]);
    exit(-1);
  }

  int inputCount = atoi(argv[1]);
  int outputCount = atoi(argv[2]);
  int sparseRatio = atoi(argv[3]);
  int iteration = atoi(argv[4]);
 
  printf ("%s: InputCount=%d OutputCount=%d SparseRatio=%d Iteration=%d\n", 
	  argv[0], inputCount, outputCount, sparseRatio, iteration);
  
  float *activations = (float*) malloc (inputCount * sizeof(float));
  float *signals = (float*) malloc (outputCount * sizeof(float));
  float *weights = (float*) malloc (inputCount * outputCount * sizeof(float));
  
  sparsify(activations, inputCount, sparseRatio);

  PinThread();

  timeval tv1, tv2;
  gettimeofday(&tv1, NULL);
  for (int i = 0; i < iteration; i++) {
    feedforward(signals, activations, weights, inputCount, outputCount);
  }
  gettimeofday(&tv2, NULL);
  OutputTimeDiff(tv1, tv2);

  free(activations);
  free(signals);
  free(weights);

  return 0;

}


