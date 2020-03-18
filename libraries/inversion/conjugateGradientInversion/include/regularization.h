#pragma once
#include "genericInput.h"
#include "inversionInterface.h"
#include "log.h"
struct Regularization
{
    double deltaSquared;
    PressureFieldSerial gradientChiNormSquared;
    PressureFieldSerial b;
    PressureFieldSerial bSquared;
    PressureFieldSerial gRegularisation;

    Regularization(Grid2D grid):
        gradientChiNormSquared (grid), b(grid), bSquared(grid), gRegularisation(grid)
    {

    }

};

