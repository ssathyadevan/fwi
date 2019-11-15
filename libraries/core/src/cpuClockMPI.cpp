#include "cpuClockMPI.h"
#include <iostream>
#include <ctime>

CpuClockMPI::CpuClockMPI() {}
CpuClockMPI::~CpuClockMPI() {}

void CpuClockMPI::Start()
{
    t_start = MPI_Wtime();
    time_t start = (time_t) t_start;
    std::cout << "Starting at " << std::asctime(std::localtime(&start)) << std::endl;
}

void CpuClockMPI::End()
{
    t_end = MPI_Wtime();
    time_t end = (time_t) t_end;
    std::cout << "Finished at " << std::asctime(std::localtime(&end)) << std::endl;
}

void CpuClockMPI::PrintTimeElapsed()
{
    std::cout << "CPU time: " << t_end - t_start;// << " seconds" << std::endl;
}
