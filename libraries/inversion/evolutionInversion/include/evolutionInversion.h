#pragma once

#include "evolutionInversionInput.h"
#include "evolutionInversionInputCardReader.h"
#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include "progressBar.h"

class EvolutionInversion : public inversionInterface
{
public:
    EvolutionInversion(forwardModelInterface *forwardModel, const EvolutionInversionInput &eiInput);

    EvolutionInversion(const EvolutionInversion &) = delete;
    EvolutionInversion &operator=(const EvolutionInversion &) = delete;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);

private:
    forwardModelInterface *_forwardModel;
    EvolutionInversionInput _eiInput;

    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup &_freq;

    std::ofstream openResidualLogFile(genericInput &gInput);
};
