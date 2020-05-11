#pragma once
#include "dataGrid2D.h"

struct ReconstructorParameters
{
public:
    double tolerance;
    double startingChi;
    int maxIterationsNumber;

    ReconstructorParameters() : tolerance(0.0), startingChi(0.0), maxIterationsNumber(0) {}
    ReconstructorParameters(double tolerance_, double startingChi_, int maxIterationsNumber_) :
        tolerance(tolerance_), startingChi(startingChi_), maxIterationsNumber(maxIterationsNumber_)
    {
    }
};
