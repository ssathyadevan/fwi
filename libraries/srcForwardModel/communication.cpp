#include "communication.h"


void ClockStart(int inputFreqNTotal)
{
    std::cout << "Preprocessing the provided input to create the reference pressure-field" << std::endl;
    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
    std::cout << "input.freq.nTotal= " << inputFreqNTotal << std::endl;
}

void ClockStop(int ret)
{
    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
}

void WriteToFileNotToTerminal(std::string runName, std::string postfix)
{
    std::cout << "Printing the program output onto a file named: "+runName+postfix+".out in the inputOutput folder" << std::endl;
    std::string tempString = "../../../parallelized-fwi/inputOutput/" + runName + postfix + ".out";
    if (freopen(tempString.c_str(),"w", stdout)) {}
}
