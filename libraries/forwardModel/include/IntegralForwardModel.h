#ifndef INTEGRALFORWARDMODEL_H
#define INTEGRALFORWARDMODEL_H

#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"

class IntegralForwardModel : public ForwardModelInterface
{

public:

    IntegralForwardModel( const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq, const forwardModelInput &fmInput );

    ~IntegralForwardModel();

    virtual void calculatePData(const pressureFieldSerial &chiEst, std::complex<double> *kOperator);

    void calculateKappa();
    virtual void calculatePTot(const pressureFieldSerial &chiEst);

    void createKappaOperator(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double>* pData);
    void createKappaOperator(const pressureFieldComplexSerial &CurrentPressureFieldComplexSerial, std::complex<double> *kOperator);

    void calculateOperatorKres(pressureFieldComplexSerial &kRes, std::complex<double>* res);

private:

    Greens_rect_2D_cpu          **_Greens;

    pressureFieldComplexSerial  ***_P0;
    pressureFieldComplexSerial  **_Ptot;
    pressureFieldComplexSerial  **_Kappa;

    void createP0();
    void deleteP0();

    void createPTot(const frequenciesGroup &freq, const sources &src);

    void createGreens();
    void deleteGreens();

    void deletePtot();

    pressureFieldComplexSerial calcTotalField(const Greens_rect_2D_cpu &G, const pressureFieldSerial &chiEst, const pressureFieldComplexSerial &Pinit);

    void createKappa(const frequenciesGroup &freq, const sources &src, const receivers &recv);
    void deleteKappa();
};

#endif
