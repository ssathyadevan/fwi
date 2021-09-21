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
        class EvolutionInversion : public inversionInterface<double>
        {
        public:
            EvolutionInversion(
                const core::CostFunctionCalculator &costCalculator, forwardModels::ForwardModelInterface<double> *forwardModel, const EvolutionInversionInput &eiInput);

            EvolutionInversion(const EvolutionInversion &) = delete;
            EvolutionInversion &operator=(const EvolutionInversion &) = delete;

            core::complexDataGrid2D<double> reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);

        private:
            forwardModels::ForwardModelInterface *_forwardModel;
            const core::CostFunctionCalculator &_costCalculator;
            EvolutionInversionInput _eiInput;

            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;

            std::ofstream openResidualLogFile(io::genericInput &gInput);
            core::complexDataGrid2D<double> createRandomChild(const core::complexDataGrid2D<double> &parent, std::default_random_engine &generator, std::normal_distribution<double> &distribution) const;
        };
    }   // namespace inversionMethods
}   // namespace fwi
