#pragma once

#include <iostream>
#include <cmath>
#include <string>
#include <sstream>
#include <vector>
#include <mpi.h>

inline void WriteToFileNotToTerminal(std::string outputLocation, std::string runName, std::string postfix)
{
  if (postfix == "Process"){
    int mpi_rank;
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);
      std::cout << "Printing the program output onto a file named: " + runName << mpi_rank << postfix + ".out in the output folder" << std::endl;
      std::stringstream ss;
      ss << outputLocation + runName << mpi_rank << postfix + ".out";
      std::string tempString = ss.str();
      if (freopen(tempString.c_str(),"w", stdout)) {}
      if (mpi_rank>0) return;
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

