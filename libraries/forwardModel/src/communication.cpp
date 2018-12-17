#include "communication.h"


void ClockStart()
{
    std::time_t start = std::time(nullptr);
    std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
}

void ClockStop()
{
    std::time_t finish = std::time(nullptr);
    std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
}

void WriteToFileNotToTerminal(std::string outputLocation, std::string cardName, std::string postfix)
{
    std::cout << "Printing the program output onto a file named: "+cardName+postfix+".out in the inputOutput folder" << std::endl;
    std::string tempString = outputLocation + cardName + postfix + ".out";
    if (freopen(tempString.c_str(),"w", stdout)) {}
}
