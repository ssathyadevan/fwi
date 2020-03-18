#pragma once
#include "genericInput.h"
#include "inversionInterface.h"
#include "log.h"
struct RegularisationParameters
{
    double deltaSquared;
    double fRegularisation;
    std::vector<PressureFieldSerial> gradientChi;
    PressureFieldSerial gradientChiNormSquared;
    PressureFieldSerial b;
    PressureFieldSerial bSquared;
    PressureFieldSerial gRegularisation;

    RegularisationParameters(Grid2D grid):
        gradientChi(2, PressureFieldSerial(grid)), gradientChiNormSquared (grid), b(grid), bSquared(grid), gRegularisation(grid)
    {

    }

};

