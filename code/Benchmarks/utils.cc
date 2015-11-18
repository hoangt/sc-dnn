#include <pthread.h>
#include "utils.h"
#include <stdlib.h>
#include <iostream>

using namespace std;


timeval timeDiff(timeval start, timeval end)
{
  timeval temp;
  if ((end.tv_usec-start.tv_usec) < 0) {
    temp.tv_sec = end.tv_sec - start.tv_sec - 1;
    temp.tv_usec = 1000000 + end.tv_usec - start.tv_usec;
  }
  else {
    temp.tv_sec = end.tv_sec - start.tv_sec;
    temp.tv_usec = end.tv_usec - start.tv_usec;
  }
  return temp;
}

void OutputTimeDiff(timeval tv1, timeval tv2)
{
  cout<<"Secs: "<<timeDiff(tv1,tv2).tv_sec<<endl<<"Usecs: "<<timeDiff(tv1,tv2).tv_usec<<endl;  
}

const int sparseLimit = 100;
void sparsify (float *buffer, int count, int sparseRatio)
{
  srand(777777);
  if (sparseRatio == 0) {
    for (int i = 0; i < count; i++) {
      buffer[i] = 1.0f * (i + 1);
    }
  }
  else {
   for (int i = 0; i < count; i++) {
     const int prob = rand() % sparseLimit; 
     buffer[i] = (prob < sparseRatio) ? 0.0f : 1.0f * i;
    }
  }
}

void PinThread()
{
  pthread_t thread = pthread_self();
  cpu_set_t cpuset;

  CPU_ZERO(&cpuset);
  CPU_SET(3, &cpuset);

  int s = pthread_setaffinity_np(thread, sizeof(cpu_set_t), &cpuset);
  if (s == 0) {
    cout << "Success: Pin Thread on CPU 3" << endl; 
  }
  else {
    cout << "Failure: Pin Thread on CPU 3" << endl;
  }
  return;
}
