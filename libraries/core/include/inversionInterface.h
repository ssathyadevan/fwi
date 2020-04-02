#pragma once

#include "pressureFieldSerial.h"
#include <complex>

class inversionInterface
{

public:
    inversionInterface() {}
    virtual ~inversionInterface() = default;

    virtual pressureFieldSerial reconstruct(const std::vector<std::complex<double>> &p_data, genericInput input) = 0;
};
