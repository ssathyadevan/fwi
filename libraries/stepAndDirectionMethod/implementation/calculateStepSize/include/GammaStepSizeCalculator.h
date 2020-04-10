#pragma once

#include "StepSizeCalculator.h"
#include "dataGrid2D.h"
#include <vector>

using namespace std;

class GammaStepSizeCalculator : public StepSizeCalculator
{
public:
    GammaStepSizeCalculator(dataGrid2D currentChi, vector<double> currentDerivative);   // To do, constructor assign
    virtual ~GammaStepSizeCalculator();

    double calculateStepSize();

private:
    dataGrid2D _currentChi;
    vector<double> _currentDerivative;

    dataGrid2D _prevChi = _currentChi;
    vector<double> _prevDerivative = _currentDerivative;

    int _stepIterator;
};
