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

    RegularisationParameters(grid2D grid) :
        deltaSquared(0.0), errorFunctional(1.0), gradientChi(2, dataGrid2D(grid)), gradientChiNormSquared(grid), b(grid), bSquared(grid),
        gradient(grid)
    {
    }
};
