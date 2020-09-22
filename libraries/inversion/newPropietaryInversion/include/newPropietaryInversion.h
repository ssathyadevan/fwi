#pragma once
#include "genericInput.h"
#include "inversionInterface.h"
#include <array>
#include <iostream>
#undef pascal
#include <units.h>

namespace fwi
{
    namespace inversionMethods
    {
        class NewPropietaryInversion : public inversionInterface
        {
        private:
			const std::array<double,2> xmin{1,2},xmax{1,2};
			const std::array<int,2> nx{1,2};

        public:
            NewPropietaryInversion();
            core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);
        };
    }   // namespace inversionMethods
}   // namespace fwi
