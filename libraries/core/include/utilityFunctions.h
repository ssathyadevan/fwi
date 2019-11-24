#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>

inline void WriteToFileNotToTerminal(std::string outputLocation, std::string runName, std::string postfix, const int rank)
{
	if (rank > 0) {
		std::cout.setstate(std::ios_base::failbit);//Supress cout output
		return;
	}
	std::cout << "Printing the program output onto a file named: " + runName + postfix + ".out in the output folder" << std::endl;
	std::string tempString = outputLocation + runName + postfix + ".out";
	if (freopen(tempString.c_str(),"w", stdout)) {}
}

inline double dist(double x, double z)
{
  return std::pow(std::pow(z, 2.0) + std::pow(x, 2.0), 0.5);
}

inline std::vector<std::string> returnInputDirectory(int c, char** v = NULL)
{
        if(c < 2) {
        	std::vector<std::string> arguments = {"./"};
            return arguments;
        }
        std::vector<std::string> arguments(v+1, c+v);
        return arguments;
}

