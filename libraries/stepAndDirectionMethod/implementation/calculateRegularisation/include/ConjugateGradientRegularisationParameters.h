#pragma once
#include "dataGrid2D.h"
#include <vector>
struct ConjugateGradientRegularisationParameters
{
    double deltaSquared;                   // updated in Current
    double errorFunctional;                // updated in Previous
    std::vector<dataGrid2D> gradientChi;   // updated in Previous
    dataGrid2D gradientChiNormSquared;     // updated in Previous
    dataGrid2D b;                          // updated in Current
    dataGrid2D bSquared;                   // updated in Current
    //    double gradientChiNormSquared;
    //    double b;
    //    double bSquared;
    dataGrid2D gradient;   // updated in Current

    ConjugateGradientRegularisationParameters(const grid2D &grid_) :
        deltaSquared(0.0), errorFunctional(1.0), gradientChi(std::vector<dataGrid2D>(2, dataGrid2D(grid_))), gradientChiNormSquared(grid_), b(grid_),
        bSquared(grid_), gradient(grid_)
    {
    }
};
