#pragma once

#include <ctime>

class cpuClock
{
private:

    clock_t t_start;
    clock_t t_end;

public:
    cpuClock();
    ~cpuClock();
    void Start();
    void End();
    void PrintTimeElapsed();
};
