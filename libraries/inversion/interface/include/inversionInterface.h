#pragma once

#include "complexDataGrid2D.h"
#include <complex>

namespace fwi
{
    namespace inversionMethods
    {
        template <class _V = double>
        class inversionInterface
        {
        public:
            inversionInterface() {}
            virtual ~inversionInterface() = default;

            virtual core::complexDataGrid2D<_V> reconstruct(const std::vector<std::complex<double>> &p_data, io::genericInput input) = 0;
        };
    }   // namespace inversionMethods
}   // namespace fwi
