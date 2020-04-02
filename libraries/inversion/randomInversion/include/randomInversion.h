#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "randomInversionInput.h"
#include "randomInversionInputCardReader.h"
#include "genericInput.h"



class RandomInversion : public inversionInterface
{

public:

    RandomInversion(forwardModelInterface *forwardModel, const RandomInversionInput& riInput);

    RandomInversion(const RandomInversion&) = delete;
    RandomInversion& operator=(const RandomInversion&) = delete;

    pressureFieldSerial reconstruct(const std::vector<std::complex<double>> &pData, genericInput gInput);

private:
    forwardModelInterface *_forwardModel;
    RandomInversionInput             _riInput;

    const grid2D &                    _grid;
    const sources &                   _src;
    const receivers &                 _recv;
    const frequenciesGroup &          _freq;
};
