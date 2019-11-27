#include "cpuClock.h"
#include <sstream>
#include "log.h"

CpuClock::CpuClock() {}
CpuClock::~CpuClock() {}

void CpuClock::Start()
{
    start = std::time(nullptr);
    L_(linfo) << "Starting at " << std::asctime(std::localtime(&start)) ;
    t_start = clock();
}

void CpuClock::End()
{
    finish = std::time(nullptr);
    L_(linfo) << "Finished at " << std::asctime(std::localtime(&finish)) ;
    t_end = clock();
}

std::string CpuClock::OutputString()
{
    L_(linfo) << "CPU time: " << (float(t_end - t_start)) / CLOCKS_PER_SEC << " seconds" ;
        // This part is needed for plotting the chi values in postProcessing.py
    std::stringstream ss;
    
    ss << "Timing:" << std::endl;
    ss << "Starting at " << std::asctime(std::localtime(&start)) << std::endl;
    ss << "Finished at " << std::asctime(std::localtime(&finish)) << std::endl;
    ss<< "CPU time: " << (float(t_end - t_start)) / CLOCKS_PER_SEC << " seconds" << std::endl;

    return ss.str();
}
