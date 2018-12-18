#ifndef UTILITYFUNCTIONS_H
#define UTILITYFUNCTIONS_H

#include <iostream>
#include <cmath>

inline void WriteToFileNotToTerminal(std::string outputLocation, std::string cardName, std::string postfix)
{
    std::cout << "Printing the program output onto a file named: "+cardName+postfix+".out in the inputOutput folder" << std::endl;
    std::string tempString = outputLocation + cardName + postfix + ".out";
    if (freopen(tempString.c_str(),"w", stdout)) {}
}

inline double dist(double x, double z)
{
  return std::pow(std::pow(z, 2.0) + std::pow(x, 2.0), 0.5);
}

#endif
