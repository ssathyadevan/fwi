#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "randomInversionInput.h"
#include "randomInversionInputCardReader.h"
#include "genericInput.h"


using std::cout;
using std::endl;


class RandomInversion : public InversionInterface
{

public:

    RandomInversion(ForwardModelInterface *forwardModel, GenericInput gInput);

    RandomInversion(const RandomInversion&) = delete;
    RandomInversion& operator=(const RandomInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::complex<double> *const pData, GenericInput gInput);

private:
    ForwardModelInterface   *_forwardModel;
    RandomInversionInput             _riInput;

    const Grid2D&                    _grid;
    const Sources&                   _src;
    const Receivers&                 _recv;
    const FrequenciesGroup&          _freq;
};
