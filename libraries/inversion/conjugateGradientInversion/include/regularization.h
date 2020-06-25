#pragma once
#include "genericInput.h"
#include "inversionInterface.h"
#include "log.h"

namespace fwi
{
    namespace inversionMethods
    {
        struct RegularisationParameters
        {
            double deltaSquared;
            double errorFunctional;
            std::vector<core::dataGrid2D> gradientChi;
            core::dataGrid2D gradientChiNormSquared;
            core::dataGrid2D b;
            core::dataGrid2D bSquared;
            core::dataGrid2D gradient;

            RegularisationParameters(const core::grid2D &grid_)
                : deltaSquared(0.0)
                , errorFunctional(1.0)
                , gradientChi(std::vector<core::dataGrid2D>(2, core::dataGrid2D(grid_)))
                , gradientChiNormSquared(grid_)
                , b(grid_)
                , bSquared(grid_)
                , gradient(grid_)
            {
            }
        };
    }   // namespace inversionMethods
}   // namespace fwi
