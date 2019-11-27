#pragma once

#include <string>
#include <ctime>

class CpuClock
{
private:

    clock_t t_start;
    clock_t t_end;
    std::time_t start;
    std::time_t finish;

public:
    CpuClock();
    ~CpuClock();
    void Start();
    void End();
    std::string OutputString();
};
