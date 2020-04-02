#pragma once

#include "freq.h"
#include <array>
#include "nSourcesReceivers.h"

struct genericInput
{
     std::string caseFolder;
     std::string inputFolder;
     std::string outputLocation;
     std::string runName;
     double c0;
     Freq freq;
     std::array<double,2> reservoirTopLeftCornerInM;
     std::array<double,2> reservoirBottomRightCornerInM;
     std::array<double,2> sourcesTopLeftCornerInM;
     std::array<double,2> sourcesBottomRightCornerInM;
     std::array<double,2> receiversTopLeftCornerInM;
     std::array<double,2> receiversBottomRightCornerInM;
	 std::array<int,2> nGridOriginal;
     std::array<int,2> nGrid;
     nSourcesReceiversStruct nSourcesReceivers;
     std::string fileName;
     bool verbose;
};

