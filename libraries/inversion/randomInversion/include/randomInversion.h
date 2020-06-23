#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include "randomInversionInput.h"
#include "randomInversionInputCardReader.h"

class RandomInversion : public inversionInterface
{
public:
    RandomInversion(forwardModels::forwardModelInterface *forwardModel, const RandomInversionInput &riInput);

    RandomInversion(const RandomInversion &) = delete;
    RandomInversion &operator=(const RandomInversion &) = delete;

    core::dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, io::genericInput gInput);

private:
    forwardModels::forwardModelInterface *_forwardModel;
    RandomInversionInput _riInput;

    const core::grid2D &_grid;
    const core::sources &_src;
    const core::receivers &_recv;
    const core::frequenciesGroup &_freq;

    std::ofstream openResidualLogFile(io::genericInput &gInput);
};
