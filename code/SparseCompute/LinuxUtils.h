#include <sys/time.h>

typedef struct timeval TimeVal;

struct MyTimer {
  TimeVal _startTimer;
  TimeVal _stopTimer;

  void StartTimer(void)
  {
    gettimeofday(&_startTimer, NULL);
  }

  void StopTimer(void)
  {
    gettimeofday(&_stopTimer, NULL);
  }

  double ElapsedUsecTime(void)
  {
    TimeVal temp;
    if ((_stopTimer.tv_usec - _startTimer.tv_usec) < 0) {
      temp.tv_sec = _stopTimer.tv_sec - _startTimer.tv_sec - 1;
      temp.tv_usec = 1000000 + _stopTimer.tv_usec - _startTimer.tv_usec;
    }
    else {
      temp.tv_sec = _stopTimer.tv_sec - _startTimer.tv_sec;
      temp.tv_usec = _stopTimer.tv_usec - _startTimer.tv_usec;
    }
    return (1000000 * temp.tv_sec) + temp.tv_usec;
  }
};

typedef struct MyTimer MyTimer;
