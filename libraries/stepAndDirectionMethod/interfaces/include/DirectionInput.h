#pragma once
#include "dataGrid2D.h"

// template<typename T>
class DirectionInput
{
public:
    //  T _specificDirectionInput;
    double _tolerance;   // for conjugatedGradient
    dataGrid2D _startingChi;
    int _maxIterationsNumber;
    double _h;

    DirectionInput(dataGrid2D startingChi);
    DirectionInput(double tolerance, dataGrid2D startingChi, int maxIterationsNumber, double h);
};
