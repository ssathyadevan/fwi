#pragma once

#include <mpi.h>
#include <string>

class CpuClockMPI
{
private:

    double t_start;
    double t_end;
    time_t start;
    time_t end;

public:
    void Start();
    void End();
    std::string OutputString();
    CpuClockMPI();
    ~CpuClockMPI();
    void DebugPrint(std::string msg);
};
