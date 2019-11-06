#pragma once

#include "pressureFieldSerial.h"
#include <complex>

class InversionInterface
{

public:
    InversionInterface() {}
    virtual ~InversionInterface() = default;

    virtual PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &p_data, GenericInput input) = 0;
};
