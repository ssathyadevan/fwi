#ifndef INVERSIONRANDOM
#define INVERSIONRANDOM

#include "inversionInterface.h"
#include "forwardModel.h"
#include "randomInversionInput.h"


using std::cout;
using std::endl;


class inversionRandom : public inversionInterface
{
private:
    ForwardModelInterface *_forwardModel;
    randomInversionInput _riInput;

public:

    inversionRandom(const inversionRandom&) = delete;
    inversionRandom& operator=(const inversionRandom&) = delete;
    inversionRandom(ForwardModelInterface *forwardModel, randomInversionInput riInput);


    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput);
};

#endif // INVERSIONRANDOM
