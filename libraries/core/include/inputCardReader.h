#ifndef READERVAR
#define READERVAR
#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <sstream>


  
//Here we determine (is_this_our_kind_of_bool) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)
class inputCardReader
{
public:
    virtual ~inputCardReader() = default;
protected:
    bool CheckBoolString(const std::string& string_for_bool) const;
    std::vector<std::string> Reader(const std::string& runName) const;
    bool InputStringToBool(std::string const& string_for_bool) const;
    virtual void readCard(std::string inputCardPath, std::string outputLocation, std::string cardName)=0;

};




#endif
