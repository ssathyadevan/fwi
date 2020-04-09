#pragma once
class dataGrid2D;

// template<typename T>
struct DirectionInput
{
    //  T _specificDirectionInput;
    double _tolerance;   // for conjugatedGradient

    dataGrid2D _startingChi;
    int _maxIterationsNumber;
    double _h;
};
