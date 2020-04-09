#pragma once
#include "genericInput.h"
#include "inversionInterface.h"
#include "log.h"
struct RegularisationParameters
{
    double deltaSquared;
    double errorFunctional;
    std::vector<dataGrid2D> gradientChi;
    dataGrid2D gradientChiNormSquared;
    dataGrid2D b;
    dataGrid2D bSquared;
    dataGrid2D gradient;

    RegularisationParameters(const grid2D &grid_) :
        deltaSquared(0.0), errorFunctional(1.0), gradientChi(std::vector<dataGrid2D>(2, dataGrid2D(grid_))), gradientChiNormSquared(grid_), b(grid_),
        bSquared(grid_), gradient(grid_)
    {
    }
};
