#include "cpuClockMPI.h"
#include <iostream>
#include <sstream>
#include <ctime>
#include "log.h"

//For memory usage
#if __unix__
    #include "stdlib.h"
    #include "stdio.h"
    #include "string.h"
#else
    #include "windows.h"
    #include "psapi.h"
#endif

namespace
{
	long parseLine(char* line){
    // This assumes that a digit will be found and the line ends in " Kb".
    int i = strlen(line);
    const char* p = line;
    while (*p <'0' || *p > '9') p++;
    line[i-3] = '\0';
    i = atoi(p);
    return i;
}

int getValue(const char* item){ //Note: this value is in KB!
    FILE* file = fopen("/proc/self/status", "r");
    int result = -1;
    char line[128];

    while (fgets(line, 128, file) != NULL){
        if (strncmp(line, item, 6) == 0){
            result = parseLine(line);
            break;
        }
    }
    fclose(file);
    return result;
}

	
} // namespace

namespace performance
{
	
CpuClockMPI::CpuClockMPI() {}
CpuClockMPI::~CpuClockMPI() {}

void CpuClockMPI::Start()
{
    t_start = MPI_Wtime();
    start = std::chrono::system_clock::now();
    L_(io::linfo) << "Starting";
}

void CpuClockMPI::End()
{
    t_end = MPI_Wtime();
    finish = std::chrono::system_clock::now();
    L_(io::linfo) << "Finished";
}

std::string CpuClockMPI::OutputString()
{
    L_(io::linfo) << "CPU time: " << (double(t_end - t_start)) << " seconds" ;
    L_(io::linfo) << "Wall time: " << double(finish.time_since_epoch().count() - start.time_since_epoch().count()) / double(1000000000) << "seconds"; //nanosec / 10^9 = sec
    long virtual_mem, physical_mem;
    MemoryUse(virtual_mem, physical_mem);
    std::stringstream ss;
    ss << "Timing:"                                                         << std::endl;
    ss << "Starting at "      << start.time_since_epoch().count()           << std::endl;
    ss << "Finished at "      << finish.time_since_epoch().count()          << std::endl;
    ss << "CPU time: "        << (double(t_end - t_start)) << std::endl;
    ss << std::endl << "Others:"                                            << std::endl;
    ss << "Virtual memory: "  << virtual_mem                                << std::endl;
    ss << "Physical memory: " << physical_mem                               << std::endl;

    return ss.str();
}

void CpuClockMPI::MemoryUse(long& virtual_mem, long& physical_mem){
    #if __unix__
        virtual_mem = getValue("VmSize:");
        physical_mem = getValue("VmRSS:");
    #else   
        PROCESS_MEMORY_COUNTERS_EX pmc;
        GetProcessMemoryInfo(GetCurrentProcess(), &pmc, sizeof(pmc));
        SIZE_T virtualMemUsedByMe = pmc.PrivateUsage;
        SIZE_T physMemUsedByMe = pmc.WorkingSetSize;
        virtual_mem = static_cast<long> virtualMemUsedByMe  / 1024 ;
        physical_mem = static_cast<long> physMemUsedByMe  / 1024 ;
    #endif
    L_(ldebug) << "Virtual memory used: " << virtual_mem<< " kB";
    L_(ldebug) << "Physical memory used: " << physical_mem<< " kB";
    return;
}

void CpuClockMPI::DebugPrint(std::string msg)
{
    std::cerr << msg << "CPU time: " << t_end - t_start << " seconds" << std::endl;
}
} // namespace performance