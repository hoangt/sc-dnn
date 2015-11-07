#pragma once

#include "BasicTypes.h"

class CHiResTimer
{
public:

    // Start timer
    void Start();

    // Stop timer
    void Stop();

    // Get elapsed seconds
    double GetElapsedSecs();

    // Get elapsed micro secs
    double GetElapsedMicroSecs();

    // Get elapsed milli secs
    double GetElapsedMillSecs();

private:

    // Constants
    static const uint32 c_MilliSeconds = 1000;
    static const uint32 c_MicroSeconds = 1000 * c_MilliSeconds;

    // Start time
    LARGE_INTEGER _start;

    // End time
    LARGE_INTEGER _stop;
}; 