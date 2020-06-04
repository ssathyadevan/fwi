#pragma once


struct PMLWidthFactor
{
    double x;
    double z;

    PMLWidthFactor() : x(0.0), z(0.0) {}
    PMLWidthFactor(int x_, double z_) : x(x_), z(z_) {}
};

struct SourceParameter
{
    int r;
    double beta;

    SourceParameter() : r(0), beta(0.0) {}
    SourceParameter(int r_, double beta_) : r(r_), beta(beta_) {}
};
