#ifndef READERVAR
#define READERVAR

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>

//#include "fwiread.cpp"

using namespace std;

vector<string> reader(){
    vector<string> thevec;
    fstream f1("inputfwi.txt");
    string line,w1;
    while ( getline(f1,line) ) {
        if (( istringstream(line) >> w1)  && ( w1[0] != '#' )) {
        thevec.push_back(w1);
        }}
    return thevec;
}
#endif
