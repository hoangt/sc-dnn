#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <iostream>
#include "utils.h"

using namespace std;

timespec diff(timespec start, timespec end)
{
	timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}


#ifdef ZERO_OPT
void backprop(float *errors, const float* signals, const float* weights, int inputCount, int outputCount)
{
  int skipCount = 0;
  for (int i = 0; i < outputCount; i++) {
    if (signals[i] != 0.0f) {
      const float* signalWeights = weights + (i * inputCount);
      for (int j = 0; j < inputCount; j++) {
	errors[j] += signals[i] * signalWeights[j];
      }
    }
    else {
	skipCount++;
    }
  }
  //  printf ("ZERO_OPT: skipped %d inner loops\n", skipCount);
}

#else
void backprop(float *errors, const float* signals, const float* weights, int inputCount, int outputCount)
{
  for (int i = 0; i < outputCount; i++) {
    const float* signalWeights = weights + (i * inputCount);
    for (int j = 0; j < inputCount; j++) {
      errors[j] += signals[i] * signalWeights[j];
    }
  }
}
#endif

int main (int argc, char** argv)
{
#if 0
  int inputCount = 100;
  int outputCount = 200;
  int sparseRatio = 2;
  int iteration = 1;
#else
  if (argc < 5) {
    printf("Usage: %s <InputCount> <OutputCount> <SparseRatio> <Iterations>\n", argv[0]);
    exit(-1);
  }
  int inputCount = atoi(argv[1]);
  int outputCount = atoi(argv[2]);
  int sparseRatio = atoi(argv[3]);
  int iteration = atoi(argv[4]);
  if (sparseRatio == 0) sparseRatio = 1;
#endif
  printf("BackProp: InputCount=%d, OutputCount=%d SparseRatio=%d Iteration=%d\n", 
	 inputCount, outputCount, sparseRatio, iteration);
  
  float *signals = (float*) malloc (outputCount * sizeof(float));
  float *errors = (float*) malloc (inputCount * sizeof(float));
  float *weights = (float*) malloc (inputCount * outputCount * sizeof(float));

  sparsify(signals, outputCount, sparseRatio);
 timespec time1, time2;
 timeval tv1, tv2;

 gettimeofday(&tv1, NULL);
 // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time1);
  for (int i = 0; i < iteration; i++) {
    backprop(errors, signals, weights, inputCount, outputCount);
   }
  // clock_gettime(CLOCK_PROCESS_CPUTIME_ID, &time2);
  gettimeofday(&tv2, NULL);
  
  //cout<<"NSecClock:"<<diff(time1,time2).tv_sec<<":"<<diff(time1,time2).tv_nsec<<endl;

  cout<<"USecClock:"<<timeDiff(tv1,tv2).tv_sec<<":"<<timeDiff(tv1,tv2).tv_usec<<endl;

  free (signals);
  free (errors);
  free (weights);

  return 0;
}
