#include "cpuClock.h"
#include <iostream>

cpuClock::cpuClock() {}
cpuClock::~cpuClock(){}

void cpuClock::Start()
{
    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    t_start = clock();
}

void cpuClock::End()
{
    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    t_end = clock();
}

void cpuClock::PrintTimeElapsed()
{
    std::cout << "CPU time: " << (float(t_end-t_start))/CLOCKS_PER_SEC << " seconds" << std::endl;
}

