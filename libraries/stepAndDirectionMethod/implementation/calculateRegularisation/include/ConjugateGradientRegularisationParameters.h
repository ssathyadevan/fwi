#pragma once
#include "dataGrid2D.h"
#include <vector>
struct ConjugateGradientRegularisationParameters
{
    double deltaSquared;
    double errorFunctional;
    std::vector<dataGrid2D> gradientChi;
    dataGrid2D gradientChiNormSquared;
    dataGrid2D b;
    dataGrid2D bSquared;
    dataGrid2D gradient;

    ConjugateGradientRegularisationParameters(const grid2D &grid_) :
        deltaSquared(0.0), errorFunctional(1.0), gradientChi(std::vector<dataGrid2D>(2, dataGrid2D(grid_))), gradientChiNormSquared(grid_), b(grid_),
        bSquared(grid_), gradient(grid_)
    {
    }
};
