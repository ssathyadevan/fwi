#pragma once

#include "dataGrid2D.h"
#include <complex>

class inversionInterface
{

public:
    inversionInterface() {}
    virtual ~inversionInterface() = default;

    virtual dataGrid2D reconstruct(const std::vector<std::complex<double>> &p_data, genericInput input) = 0;
};
