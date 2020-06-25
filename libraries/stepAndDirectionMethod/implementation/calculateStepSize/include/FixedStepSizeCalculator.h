#pragma once

#include "StepSizeCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        class FixedStepSizeCalculator : public StepSizeCalculator
        {
        public:
            FixedStepSizeCalculator(double size);
            virtual ~FixedStepSizeCalculator();

            double calculateStepSize();
            void updateVariables(
                const core::dataGrid2D &, const core::dataGrid2D &, int, const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &);

        private:
            double _size = 0;
        };
    }   // namespace inversionMethods
}   // namespace fwi
