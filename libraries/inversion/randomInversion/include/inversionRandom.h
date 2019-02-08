#ifndef INVERSIONRANDOM
#define INVERSIONRANDOM

#include "inversionInterface.h"
#include "forwardModel.h"


using std::cout;
using std::endl;


class inversionRandom : public inversionInterface
{
private:
    ForwardModelInterface *forwardModel_;

public:
    inversionRandom(const inversionRandom&) = delete;
    inversionRandom& operator=(const inversionRandom&) = delete;

    inversionRandom(ForwardModelInterface *forwardModel)
    {
        forwardModel_ = forwardModel;
    }

    ~inversionRandom()
    {
    }

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, Input input);
};

#endif // INVERSIONRANDOM
