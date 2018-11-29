#include "ProfileCpu.h"

#include <string>

ProfileCpu::ProfileCpu()
    : T_start(std::chrono::system_clock::now()), T_end(std::chrono::system_clock::now()), regionname("")
{

}

void ProfileCpu::StartRegion(const std::string &name)
{
  regionname = name;
  T_start = std::chrono::system_clock::now();
}

void ProfileCpu::EndRegion()
{
    T_end = std::chrono::system_clock::now();
    std::chrono::duration<double> elapsed_secs = T_end - T_start;
    std::cout << "Computation time for " << regionname << " was " << elapsed_secs.count() << " seconds" << std::endl;
}
