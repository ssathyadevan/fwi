#pragma once

struct iter1
{
    int n;
    double tolerance;

    iter1() : n(0), tolerance(0.0) {}
    iter1(int nrOfIterations, double tolerance_) : n(nrOfIterations), tolerance(tolerance_) {}
};
