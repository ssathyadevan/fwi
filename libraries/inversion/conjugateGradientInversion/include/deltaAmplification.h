#pragma once

struct DeltaAmplification
{
    double start;
    double slope;

    DeltaAmplification() : start(0.0), slope(0.0){};
    DeltaAmplification(double start_, double slope_) : start(start_), slope(slope_){};
};
