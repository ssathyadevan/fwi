#pragma once

#include "StepSizeCalculator.h"

namespace fwi
{
    namespace inversionMethods
    {
        class BorzilaiBorweinStepSizeCalculator : public StepSizeCalculator
        {
        private:
            std::vector<double> _chiEstimatePrevious;
            std::vector<double> _chiEstimateCurrent;

            std::vector<double> _derivativePrevious;
            std::vector<double> _derivativeCurrent;

            const double _initialStepSize;
            int _iteration;
            int _nGridPoints;

        public:
            BorzilaiBorweinStepSizeCalculator(const core::grid2D &grid, const double initialStepSize);
            virtual ~BorzilaiBorweinStepSizeCalculator();

            double calculateStepSize();
            void updateVariables(const core::dataGrid2D &chiEstimateCurrent, const core::dataGrid2D &derivativeCurrent, int iteration,
                const std::vector<std::complex<double>> &, const std::vector<std::complex<double>> &);
        };
    }   // namespace inversionMethods
}   // namespace fwi
