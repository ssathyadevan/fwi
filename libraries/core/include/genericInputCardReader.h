#pragma once
#include <vector>
#include<string>
#include "inputCardReader.h"
#include "genericInput.h"
#include <fstream>
#include <sstream>


//Here we determine (is_this_our_kind_of_bool) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)
class genericInputCardReader: public inputCardReader
{   
public:
    genericInputCardReader(std::string inputCardPath, std::string outputLocation, std::string cardName);
    std::vector<std::string> Reader(const std::string& runName) const;
    genericInput getInput();

private:
    genericInput readCard(std::string inputCardPath, std::string outputLocation, std::string cardName);

    genericInput mInput;
};




