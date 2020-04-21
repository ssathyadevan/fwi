#pragma once
#include "dataGrid2D.h"

// template<typename T>
struct DirectionInput
{
public:
    //  T _specificDirectionInput;
    const double _tolerance;
    const dataGrid2D &_startingChi;
    const int _maxIterationsNumber;
    const double _discretizationUnit;
};
