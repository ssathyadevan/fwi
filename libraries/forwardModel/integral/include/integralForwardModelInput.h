#pragma once

struct integralForwardModelInput
{
        int nrOfIterations;
        double tolerance;
        bool calcAlpha;

        integralForwardModelInput() : nrOfIterations(0), tolerance(0.0), calcAlpha(false) {}
        integralForwardModelInput(int nrOfIterations_, double tolerance_, bool calcAlpha_) : nrOfIterations(nrOfIterations_), tolerance(tolerance_), calcAlpha(calcAlpha_) {}
};
