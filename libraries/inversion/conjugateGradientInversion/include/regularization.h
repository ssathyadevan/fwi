#pragma once
#include "genericInput.h"
#include "inversionInterface.h"
#include "log.h"
struct RegularisationParameters
{
    double deltaSquared;
    double errorFunctional;
    std::vector<pressureFieldSerial> gradientChi;
    pressureFieldSerial gradientChiNormSquared;
    pressureFieldSerial b;
    pressureFieldSerial bSquared;
    pressureFieldSerial gradient;

    RegularisationParameters(grid2D grid) :
        deltaSquared(0.0), errorFunctional(1.0), gradientChi(2, pressureFieldSerial(grid)), gradientChiNormSquared(grid), b(grid), bSquared(grid),
        gradient(grid)
    {
    }
};
