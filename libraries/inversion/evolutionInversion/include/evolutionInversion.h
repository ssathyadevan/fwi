#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "evolutionInversionInput.h"
#include "evolutionInversionInputCardReader.h"
#include "genericInput.h"
#include "progressBar.h"


class EvolutionInversion : public InversionInterface
{

public:

    EvolutionInversion(ForwardModelInterface *forwardModel, GenericInput gInput);

    EvolutionInversion(const EvolutionInversion&) = delete;
    EvolutionInversion& operator=(const EvolutionInversion&) = delete;

    PressureFieldSerial Reconstruct(const std::vector<std::complex<double>> &pData, GenericInput gInput);

private:
    ForwardModelInterface           *_forwardModel;
    EvolutionInversionInput             _eiInput;

    const Grid2D&                    _grid;
    const Sources&                   _src;
    const Receivers&                 _recv;
    const FrequenciesGroup&          _freq;
};
