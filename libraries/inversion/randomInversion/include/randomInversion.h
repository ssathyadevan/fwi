#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "randomInversionInput.h"
#include "randomInversionInputCardReader.h"
#include "genericInput.h"



class RandomInversion : public InversionInterface
{

public:

    RandomInversion(ForwardModelInterface *forwardModel, GenericInput gInput);

    RandomInversion(const RandomInversion&) = delete;
    RandomInversion& operator=(const RandomInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput);

private:
    ForwardModelInterface           *_forwardModel;
    RandomInversionInput             _riInput;

    const Grid2D&                    _grid;
    const Sources&                   _src;
    const Receivers&                 _recv;
    const FrequenciesGroup&          _freq;
};
