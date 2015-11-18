#include <sys/time.h>

timeval timeDiff(timeval start, timeval end);
void sparsify (float *buffer, int count, int sparseRatio);
void OutputTimeDiff(timeval, timeval);
void   PinThread();
