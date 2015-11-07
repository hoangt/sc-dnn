#include "HiResTimer.h"

void CHiResTimer::Start()
{
    QueryPerformanceCounter(&_start);
}

void CHiResTimer::Stop()
{
    QueryPerformanceCounter(&_stop);
}

double CHiResTimer::GetElapsedSecs()
{
    LARGE_INTEGER frequency;
    QueryPerformanceFrequency(&frequency); 
    return ((double)(_stop.QuadPart - _start.QuadPart)/(double)frequency.QuadPart);
}

double CHiResTimer::GetElapsedMicroSecs()
{
    return GetElapsedSecs() * c_MicroSeconds;
}

double CHiResTimer::GetElapsedMillSecs()
{
    return GetElapsedSecs() * c_MilliSeconds;
}