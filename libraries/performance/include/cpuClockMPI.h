#pragma once

#include <chrono>
#include <mpi.h>
#include <string>

namespace performance
{
    class CpuClockMPI
    {
    private:
        double t_start;
        double t_end;
        std::chrono::system_clock::time_point start;
        std::chrono::system_clock::time_point finish;

    public:
        void Start();
        void End();
        void MemoryUse(long &virtual_mem, long &physical_mem);
        std::string OutputString();
        CpuClockMPI();
        ~CpuClockMPI();
        void DebugPrint(std::string msg);
    };
}   // namespace performance
