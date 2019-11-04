#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "evolutionInversionInput.h"
#include "evolutionInversionInputCardReader.h"
#include "genericInput.h"


using std::cout;
using std::endl;


class EvolutionInversion : public InversionInterface
{

public:

    EvolutionInversion(ForwardModelInterface *forwardModel, GenericInput gInput);

    EvolutionInversion(const EvolutionInversion&) = delete;
    EvolutionInversion& operator=(const EvolutionInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::complex<double> *const pData, GenericInput gInput);

private:
    ForwardModelInterface   *_forwardModel;
    EvolutionInversionInput             _riInput;

    const Grid2D&                    _grid;
    const Sources&                   _src;
    const Receivers&                 _recv;
    const FrequenciesGroup&          _freq;
};
