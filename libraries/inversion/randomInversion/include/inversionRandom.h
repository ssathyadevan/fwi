#ifndef INVERSIONRANDOM
#define INVERSIONRANDOM

#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "randomInversionInput.h"


using std::cout;
using std::endl;


class inversionRandom : public inversionInterface
{

public:

    inversionRandom(ForwardModelInterface *forwardModel, randomInversionInput riInput);

    inversionRandom(const inversionRandom&) = delete;
    inversionRandom& operator=(const inversionRandom&) = delete;

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput);

private:
    ForwardModelInterface   *_forwardModel;
    randomInversionInput             _riInput;

    const grid2D&                    _grid;
    const sources&                   _src;
    const receivers&                 _recv;
    const frequenciesGroup&          _freq;
};

#endif // INVERSIONRANDOM
