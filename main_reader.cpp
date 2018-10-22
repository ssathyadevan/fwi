#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>
#include "read.h"

using namespace std;

/*
   MELISSEN 2018 Oct 10 at 17h00

   This code is written to read from a simple .txt all variables required to run FWI
   As you can see we start by opening the file inputfwi.txt and reading from it.
   Only the non-hashtag-started lines are considered, the rest is treated as comment  
*/

vector<string> reader(); //defined in read_input_fwi_into_vec.cpp and declared in read.h

int main()
{

    //We read all lines and store relevant values in a big vector of strings input_parameters 

    vector<string> input_parameters = reader();

    //We transfer what is in input_parameters to the relevant constants
    int parameterCounter=0;
    const double    c_0                          = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    c_1                          = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    f_min                        = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    f_max                        = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       nxt                          = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       nzt                          = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       nSrct                        = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const int       nFreq                        = stoi(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    tol1                         = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    tol2                         = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    F_min1                       = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    F_max1                       = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    delta_amplification_start    = stod(input_parameters[parameterCounter]);    ++parameterCounter;
    const double    delta_amplification_slope    = stod(input_parameters[parameterCounter]);
    
    int hi[2][2] = {{1,2},{3,4}};
    cout <<  hi[0][0] << " " << hi[0][1] << " " << hi[1][0] << " " << hi[1][1];
    //We use couts to test whether all the values landed in the proper places NOT VERIFIED

    ofstream outputfwi;
    outputfwi.open("outputfwi.txt"); 
    outputfwi << "This run was parametrized as follows:" << endl;
    outputfwi << "c_0   = " << c_0    << endl;
    outputfwi << "c_1   = " << c_1    << endl;
    outputfwi << "f_min = " << f_min  << endl;
    outputfwi << "f_max = " << f_max  << endl;
    outputfwi << "nxt   = " << nxt    << endl;
    outputfwi << "nzt   = " << nzt    << endl;
    outputfwi << "nSrct = " << nSrct  << endl;
    outputfwi << "nFreq = " << nFreq  << endl;
    outputfwi << "tol1  = " << tol1   << endl;
    outputfwi << "tol2  = " << tol2   << endl;
    outputfwi << "F_min1= " << F_min1 << endl;
    outputfwi << "F_max1= " << F_max1 << endl;
    outputfwi << "delast= " << delta_amplification_start << endl;
    outputfwi << "delasp= " << delta_amplification_slope << endl;
    outputfwi.close();
    
    return 0;
}
