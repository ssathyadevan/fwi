#pragma once

#include "CostFunctionCalculator.h"
#include "ForwardModelInterface.h"
#include "evolutionInversionInput.h"
#include "evolutionInversionInputCardReader.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include "progressBar.h"

namespace fwi
{
    namespace inversionMethods
    {
        class EvolutionInversion : public inversionInterface
        {
        public:
            EvolutionInversion(forwardModels::ForwardModelInterface *forwardModel, const EvolutionInversionInput &eiInput);

            EvolutionInversion(const EvolutionInversion &) = delete;
            EvolutionInversion &operator=(const EvolutionInversion &) = delete;

            core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);

        private:
            forwardModels::ForwardModelInterface *_forwardModel;
            core::CostFunctionCalculator costCalculator;
            EvolutionInversionInput _eiInput;

            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;

            std::ofstream openResidualLogFile(io::genericInput &gInput);
        };
    }   // namespace inversionMethods
}   // namespace fwi
