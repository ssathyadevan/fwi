#ifndef READERVAR
#define READERVAR
#include <vector>
#include <string>



  
//Here we determine (is_this_our_kind_of_bool) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)
class inputCardReader
{
public:
    virtual std::vector<std::string> Reader(const std::string& runName) const = 0;
    virtual ~inputCardReader() = default;
protected:
    bool IsThisOurKindOfBool(const std::string& string_for_bool) const;

    bool String1ForTrue0ForFalse(std::string const& string_for_bool) const;

};




#endif
