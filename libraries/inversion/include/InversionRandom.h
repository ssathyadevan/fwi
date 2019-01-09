#ifndef INVERSIONRANDOM
#define INVERSIONRANDOM

#include "inversionInterface.h"
#include "forwardModel.h"


using std::cout;
using std::endl;


class InversionRandom : public InversionInterface
{
private:
    ForwardModelInterface *forwardModel_;

public:
    InversionRandom(const InversionRandom&) = delete;
    InversionRandom& operator=(const InversionRandom&) = delete;

    InversionRandom(ForwardModelInterface *forwardModel)
    {
        forwardModel_ = forwardModel;
    }

    ~InversionRandom()
    {
    }

    volField_rect_2D_cpu Reconstruct(const std::complex<double> *const pData, Input input);
};

#endif // INVERSIONRANDOM
