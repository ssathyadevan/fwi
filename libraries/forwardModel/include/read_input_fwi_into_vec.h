#ifndef READERVAR
#define READERVAR

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>
#include "variable_structure.h"

using namespace std;
  
//Here we determine (is_this_our_kind_of_bool) if the reader correctly gave us a Boolean (1 or 0)
//and then set the Boolean from the input string (string_1_for_true_0_for_false)

bool is_this_our_kind_of_bool(std::string const& string_for_bool);

bool string_1_for_true_0_for_false(std::string const& string_for_bool);

vector<string> reader(std::string runName);

struct Input reader3(int argc, char** argv);

#endif
