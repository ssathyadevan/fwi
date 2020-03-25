#pragma once
#include "genericInput.h"
#include "inversionInterface.h"
#include "log.h"
struct RegularisationParameters
{
    double deltaSquared;
    double errorFunctional;
    std::vector<PressureFieldSerial> gradientChi;
    PressureFieldSerial gradientChiNormSquared;
    PressureFieldSerial b;
    PressureFieldSerial bSquared;
    PressureFieldSerial gradient;

    RegularisationParameters(Grid2D grid) :
        deltaSquared(0.0), errorFunctional(1.0), gradientChi(2, PressureFieldSerial(grid)), gradientChiNormSquared(grid), b(grid), bSquared(grid),
        gradient(grid)
    {
    }
};
