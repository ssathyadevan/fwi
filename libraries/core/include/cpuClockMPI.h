#pragma once

#include <mpi.h>
#include <string>

class CpuClockMPI
{
private:

    double t_start;
    double t_end;

public:
    CpuClockMPI();
    ~CpuClockMPI();
    void Start();
    void End();
    void PrintTimeElapsed();
    void DebugPrint(std::string msg);
};
