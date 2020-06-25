#pragma once

#include "evolutionInversionInput.h"
#include "evolutionInversionInputCardReader.h"
#include "forwardModelInterface.h"
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
            EvolutionInversion(forwardModels::forwardModelInterface *forwardModel, const EvolutionInversionInput &eiInput);

            EvolutionInversion(const EvolutionInversion &) = delete;
            EvolutionInversion &operator=(const EvolutionInversion &) = delete;

            core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);

        private:
            forwardModels::forwardModelInterface *_forwardModel;
            EvolutionInversionInput _eiInput;

            const core::grid2D &_grid;
            const core::sources &_source;
            const core::receivers &_receiver;
            const core::frequenciesGroup &_freq;

            std::ofstream openResidualLogFile(io::genericInput &gInput);
        };
    }   // namespace inversionMethods
}   // namespace fwi
