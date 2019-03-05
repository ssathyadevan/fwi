#ifndef INVERSIONRANDOM
#define INVERSIONRANDOM

#include "inversionInterface.h"
#include "forwardModelBasicOptimization.h"
#include "randomInversionInput.h"


using std::cout;
using std::endl;


class inversionRandom : public inversionInterface
{
private:
    ForwardModelInterface *m_forwardModel;
    randomInversionInput m_riInput;

    const grid2D& m_grid;
    const sources& m_src;
    const receivers& m_recv;
    const frequenciesGroup& m_freq;

public:

    inversionRandom(ForwardModelInterface *forwardModel, randomInversionInput riInput);

    inversionRandom(const inversionRandom&) = delete;
    inversionRandom& operator=(const inversionRandom&) = delete;

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput);
};

#endif // INVERSIONRANDOM
