#ifndef INVERSION_CPU
#define INVERSION_CPU

#include "inversionInterface.h"
#include "forwardModel.h"


using std::cout;
using std::endl;


class inversion : public inversionInterface
{
private:
    ForwardModelInterface *forwardModel_;

public:
    inversion(const inversion&) = delete;
    inversion& operator=(const inversion&) = delete;

    inversion(ForwardModelInterface *forwardModel)
    {
        forwardModel_ = forwardModel;
    }

    ~inversion()
    {
    }

    double findRealRootFromCubic(double a, double b, double c, double d);
    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, Input input);
};

#endif // INVERSION_CPU

