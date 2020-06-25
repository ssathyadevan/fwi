#pragma once

#include "dataGrid2D.h"
#include <complex>

namespace inversionMethods
{
    class inversionInterface
    {
    public:
        inversionInterface() {}
        virtual ~inversionInterface() = default;

        virtual core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &p_data, io::genericInput input) = 0;
    };
}   // namespace inversionMethods
