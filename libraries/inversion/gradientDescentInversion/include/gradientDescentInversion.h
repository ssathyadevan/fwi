#ifndef INVERSION_CPU
#define INVERSION_CPU
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include "gradientDescentInversionInput.h"
class gradientDescentInversion : public inversionInterface{

private:

    ForwardModelInterface* _forwardModel;
    gradientDescentInversionInput _gdInput;

    const grid2D& _grid;
    const sources& _src;
    const receivers& _recv;
    const frequenciesGroup _freq;

 public:
    gradientDescentInversion(ForwardModelInterface *forwardModel, const gradientDescentInversionInput &gdInput);

    gradientDescentInversion(const gradientDescentInversion&) = delete;
    gradientDescentInversion & operator = (const gradientDescentInversion&) = delete;

    pressureFieldSerial Reconstruct(const std::complex<double> *const pData, genericInput gInput );
    std::vector<double> differential(const std::complex<double> *const pData, pressureFieldSerial xi, double dxi);
    pressureFieldSerial gradientDescent(const std::complex<double> *const pData, pressureFieldSerial xi);
    double functionF(pressureFieldSerial xi, const std::complex<double> *const pData);
};

#endif // INVERSION_CPU
