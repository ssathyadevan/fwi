#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include <iostream>
#include <string>
#include <ctime>

void ClockStart(int inputFreqNTotal);

void ClockStop(int ret);

void WriteToFileNotToTerminal(std::string outputLocation, std::string cardName, std::string postfix);

#endif
