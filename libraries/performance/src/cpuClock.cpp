#include "cpuClock.h"
#include "log.h"
#include <sstream>

// For memory usage
#if __win32__
#include "psapi.h"
#include "windows.h"
#else
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include "iostream"
#endif

namespace
{

#if __win32__
	bool makeUnixSpecificCall = false;	
#else
    bool makeUnixSpecificCall = true;
#endif

    long parseLine(char *line)
    {
        // This assumes that a digit will be found and the line ends in " Kb".
        int i = strlen(line);
        const char *p = line;
        while(*p < '0' || *p > '9')
            p++;
        line[i - 3] = '\0';
        i = atoi(p);
        return i;
    }

    int getValue(const char *item)
    {   // Note: this value is in KB!
        FILE *file = fopen("/proc/self/status", "r");
        int result = -1;
        char line[128];
        std::cout << "inside break statement1 \n";
        while(fgets(line, 128, file) != NULL)
        {
            if(strncmp(line, item, 6) == 0)
            {
                std::cout << "inside break statement2 \n";    
                result = parseLine(line);
                break;
            }
        }
        std::cout << "inside break statement \n";
        fclose(file);
        return result;
    }

}   // namespace


namespace fwi
{
    namespace performance
    {
        CpuClock::CpuClock() {}
        CpuClock::~CpuClock() {}

        void CpuClock::Start()
        {
            std::cout << "inside clock \n";
            start = std::chrono::system_clock::now();
            L_(io::linfo) << "Starting";
            long dummy;
            std::cout << "final clock \n";
            MemoryUse(dummy, dummy);
            t_start = clock();
        }

        void CpuClock::End()
        {
            finish = std::chrono::system_clock::now();
            L_(io::linfo) << "Finished";
            long dummy;
            MemoryUse(dummy, dummy);
            t_end = clock();
        }

        std::string CpuClock::OutputString()
        {
            L_(io::linfo) << "CPU time: " << (double(t_end - t_start)) / CLOCKS_PER_SEC << " seconds";
            L_(io::linfo) << "Wall time: " << double(finish.time_since_epoch().count() - start.time_since_epoch().count()) / double(1000000000)
                          << "seconds";   // nanosec / 10^9 = sec
            long virtual_mem, physical_mem;
            MemoryUse(virtual_mem, physical_mem);
            std::stringstream ss;
            ss << "Timing:" << std::endl;
            ss << "Starting at " << start.time_since_epoch().count() << std::endl;
            ss << "Finished at " << finish.time_since_epoch().count() << std::endl;
            ss << "CPU time: " << (double(t_end - t_start)) / CLOCKS_PER_SEC << std::endl;
            ss << std::endl << "Others:" << std::endl;
            ss << "Virtual memory: " << virtual_mem << std::endl;
            ss << "Physical memory: " << physical_mem << std::endl;

            return ss.str();
        }

        void CpuClock::MemoryUse(long &virtual_mem, long &physical_mem)
        {
			virtual_mem = 0;
            physical_mem = 0;
			if(makeUnixSpecificCall)
			{
			    virtual_mem = getValue("VmSize:");
                std::cout << "inside unix statement \n";
				physical_mem = getValue("VmRSS:");	
                
			}

            L_(io::ldebug) << "Virtual memory used: " << virtual_mem << " kB";
            L_(io::ldebug) << "Physical memory used: " << physical_mem << " kB";
            return;
        }

    }   // namespace performance
}   // namespace fwi
