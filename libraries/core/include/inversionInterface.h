#pragma once

#include "pressureFieldSerial.h"
#include <complex>

class inversionInterface
{

public:
    inversionInterface() {}
    virtual ~inversionInterface() = default;

    virtual pressureFieldSerial Reconstruct(const std::complex<double> *const p_data, genericInput input) = 0;
};
