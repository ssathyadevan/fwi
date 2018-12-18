#ifndef VARSTRUCT 
#define VARSTRUCT

#include <fstream>
#include <iostream>
#include <string>
#include <array>
#include <sstream>
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
struct Iter2
{
    int n;    double tolerance;   bool calcAlpha;
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
    std::string inputCardPath;
    std::string outputLocation;
    std::string cardName;
    double c_0;
    int n_max;
    bool doReg;
    bool verbose;
    DeltaAmplification deltaAmplification;
    Freq freq;
    Iter2 iter2;
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
