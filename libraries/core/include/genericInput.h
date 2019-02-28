#pragma once
#include "freq.h"
#include <array>
#include "nSourcesReceivers.h"

struct genericInput
{
     std::string inputCardPath;
     std::string outputLocation;
     std::string runName;
     double c_0;
     Freq freq;
     std::array<double,2> reservoirTopLeftCornerInM;
     std::array<double,2> reservoirBottomRightCornerInM;
     std::array<double,2> sourcesTopLeftCornerInM;
     std::array<double,2> sourcesBottomRightCornerInM;
     std::array<int,2> ngrid;
     NSourcesReceivers nSourcesReceivers;
     std::string fileName;
     bool verbose;

};

