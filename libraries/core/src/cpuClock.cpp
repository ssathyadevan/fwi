#include "cpuClock.h"
#include <iostream>

CpuClock::CpuClock() {}
CpuClock::~CpuClock() {}

void CpuClock::Start()
{
    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " << std::asctime(std::localtime(&start)) << std::endl;
    t_start = clock();
}

void CpuClock::End()
{
    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " << std::asctime(std::localtime(&finish)) << std::endl;
    t_end = clock();
}

void CpuClock::PrintTimeElapsed()
{
    std::cout << "CPU time: " << (float(t_end - t_start)) / CLOCKS_PER_SEC << " seconds" << std::endl;
}
