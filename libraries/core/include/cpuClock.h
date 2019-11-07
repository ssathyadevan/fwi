#pragma once

#include <ctime>
#include <string>

class CpuClock
{
private:

    clock_t t_start;
    clock_t t_end;

public:
    CpuClock();
    ~CpuClock();
    void Start();
    void End();
    void PrintTimeElapsed();
    void PrintTimeElapsedWithMessage(std::string msg);
};
