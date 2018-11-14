#ifndef READERVAR
#define READERVAR

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>


using namespace std;

vector<string> reader(std::string runName)
{
    vector<string> thevec;
    fstream f1(runName);
    string line,w1;
    while ( getline(f1,line) ) 
    {
        if (( istringstream(line) >> w1)  && ( w1[0] != '#' ))
	{
		thevec.push_back(w1);
	}
    }
    return thevec;
}
#endif
