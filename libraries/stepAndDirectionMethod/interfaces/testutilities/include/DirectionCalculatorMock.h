#pragma once

#include "DirectionCalculator.h"

class DirectionCalculatorMock : public DirectionCalculator
{
public:
    DirectionCalculatorMock(double errorFunctionalScalingfactor, forwardModelInterface *forwardModel);
    virtual ~DirectionCalculatorMock();
    dataGrid2D &calculateDirection(const dataGrid2D &, const std::vector<std::complex<double>> &) override;
};
