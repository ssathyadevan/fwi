#ifndef PROFILECPU_H
#define PROFILECPU_H

#include <iostream>
#include <string>
#include <vector>
#include <chrono>

class ProfileCpu{
	
public:
    ProfileCpu();
	virtual void StartRegion(const std::string &name);
	virtual void EndRegion();

	virtual ~ProfileCpu() {}


private:
    std::chrono::time_point<std::chrono::system_clock> T_start;
    std::chrono::time_point<std::chrono::system_clock> T_end;
    std::string regionname;

};

#endif
