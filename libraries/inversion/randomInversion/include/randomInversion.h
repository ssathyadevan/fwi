#pragma once

#include "CostFunctionCalculator.h"
#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include "randomInversionInput.h"
#include "randomInversionInputCardReader.h"

namespace fwi
{
    namespace inversionMethods
    {
        class RandomInversion : public inversionInterface
        {
        public:
            RandomInversion(forwardModels::forwardModelInterface *forwardModel, const RandomInversionInput &riInput);

            RandomInversion(const RandomInversion &) = delete;
            RandomInversion &operator=(const RandomInversion &) = delete;

            core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);

        private:
            forwardModels::forwardModelInterface *_forwardModel;
            core::CostFunctionCalculator _costCalculator;
            RandomInversionInput _riInput;

            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;

            std::ofstream openResidualLogFile(io::genericInput &gInput);
        };
    }   // namespace inversionMethods
}   // namespace fwi
