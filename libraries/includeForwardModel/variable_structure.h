#ifndef VARSTRUCT 
#define VARSTRUCT

#include <fstream>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <ios>
#include <vector>


using namespace std;

struct DeltaAmplification
{
    double start; double slope;
};
struct Freq
{
    double min;   double max;         int nTotal;      double spacing;
};
struct ConjGrad
{
    int nIter;    double tolerance;   bool calcAlpha;
};
struct Iter1
{
    int n;        double tolerance;
};
struct NSourcesReceivers
{
    int src;      int rec;
};
struct Input
{
    std::string pathName;
    std::string runName;
    double c_0;
    int n_max;
    bool doReg;
    bool verbose;
    DeltaAmplification deltaAmplification;
    Freq freq;
    ConjGrad conjGrad;
    Iter1 iter1;
    NSourcesReceivers nSourcesReceivers;
    std::array<int,2> ngrid;
    std::string fileName;
    std::array<double,2> reservoirTopLeftCornerInM;
    std::array<double,2> reservoirBottomRightCornerInM;
    std::array<double,2> sourcesTopLeftCornerInM;
    std::array<double,2> sourcesBottomRightCornerInM;
};
#endif