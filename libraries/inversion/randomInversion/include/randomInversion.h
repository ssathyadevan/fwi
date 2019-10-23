#pragma once

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "randomInversionInput.h"
#include "randomInversionInputCardReader.h"
#include "genericInput.h"


using std::cout;
using std::endl;


class randomInversion : public inversionInterface
{

public:

    randomInversion(ForwardModelInterface *forwardModel, genericInput gInput);

    randomInversion(const randomInversion&) = delete;
    randomInversion& operator=(const randomInversion&) = delete;

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput);

private:
    ForwardModelInterface   *_forwardModel;
    randomInversionInput             _riInput;

    const grid2D&                    _grid;
    const sources&                   _src;
    const receivers&                 _recv;
    const frequenciesGroup&          _freq;
};
