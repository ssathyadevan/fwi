#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include "randomInversionInput.h"
#include "randomInversionInputCardReader.h"

class RandomInversion : public inversionInterface
{
public:
    RandomInversion(forwardModelInterface *forwardModel, const RandomInversionInput &riInput);

    RandomInversion(const RandomInversion &) = delete;
    RandomInversion &operator=(const RandomInversion &) = delete;

    dataGrid2D reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);

private:
    forwardModelInterface *_forwardModel;
    RandomInversionInput _riInput;

    const grid2D &_grid;
    const sources &_src;
    const receivers &_recv;
    const frequenciesGroup &_freq;

    std::ofstream openResidualLogFile(genericInput &gInput);
};
