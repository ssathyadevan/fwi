#ifndef COMMUNICATION_H 
#define COMMUNICATION_H

#include <iostream>
#include <string>
#include <ctime>  

    void ClockStart(int inputFreqNTotal){
        cout << "Preprocessing the provided input to create the reference pressure-field" << endl;
        std::time_t start = std::time(nullptr);
        std::cout << "Starting at " <<  std::asctime(std::localtime(&start)) << std::endl;
        std::cout << "input.freq.nTotal= " << inputFreqNTotal << std::endl;
    }

    void ClockStop(int ret){
        std::cout << ret << std::endl;
        std::time_t finish = std::time(nullptr);
        std::cout << "Finished at " <<  std::asctime(std::localtime(&finish)) << std::endl;
    }

    void WriteToFileNotToTerminal(std::string runName, std::string postfix){
        std::cout << "Printing the program output onto a file named: "+runName+postfix+".out in the inputOutput folder" << std::endl;
        std::string tempString = "../inputOutput/" + runName + postfix + ".out";
        if (freopen(tempString.c_str(),"w", stdout)) {}
    }
#endif
