#include "cpuClockMPI.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include "log.h"

CpuClockMPI::CpuClockMPI() {}
CpuClockMPI::~CpuClockMPI() {}

void CpuClockMPI::Start()
{
    t_start = MPI_Wtime();
    start = (time_t) t_start;
    L_(linfo) << "Starting at " << std::asctime(std::localtime(&start)) ;
}

void CpuClockMPI::End()
{
    t_end = MPI_Wtime();
    end = (time_t) t_end;
    L_(linfo) << "Finished at " << std::asctime(std::localtime(&end)) ;
}

std::string CpuClockMPI::OutputString()
{
    L_(linfo) << "CPU time: " << (float(t_end - t_start)) << " seconds" ;
        // This part is needed for plotting the chi values in postProcessing.py
    std::stringstream ss;
    ss << std::endl << "Timing:" << std::endl;
    ss << "Starting at " << std::asctime(std::localtime(&start));
    ss << "Finished at " << std::asctime(std::localtime(&end));
    ss << "CPU time: " << (float(t_end - t_start)) << " seconds" << std::endl;
    
    return ss.str();
}

void CpuClockMPI::DebugPrint(std::string msg)
{
    std::cerr << msg << "CPU time: " << t_end - t_start << " seconds" << std::endl;
}
