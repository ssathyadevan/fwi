#pragma once

#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"
#include "finiteDifferenceForwardModelInput.h"
#include "genericInput.h"

class FiniteDifferenceForwardModel : public ForwardModelInterface
{

public:

    FiniteDifferenceForwardModel( const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq, const genericInput &gInput );

    ~FiniteDifferenceForwardModel();

    virtual void calculatePData(const pressureFieldSerial &chiEst, std::complex<double> *kOperator);

    void calculateKappa();
    virtual void calculatePTot(const pressureFieldSerial &chiEst);
    virtual void getUpdateDirectionInformation(std::complex<double>* res, pressureFieldComplexSerial &kRes);
    virtual void getResidualGradient(std::complex<double>* res, pressureFieldComplexSerial &kRes);
    virtual void mapDomainToSignal(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double> *kOperator);


    //void createKappaOperator(const pressureFieldComplexSerial &CurrentPressureFieldComplexSerial, std::complex<double> *kOperator);

private:

    Greens_rect_2D_cpu          **_Greens;

    pressureFieldComplexSerial  ***_p0;
    pressureFieldComplexSerial  **_pTot;
    pressureFieldComplexSerial  **_Kappa;
    finiteDifferenceForwardModelInput _fmInput;

    void createP0();
    void deleteP0();

    void createPTot(const frequenciesGroup &freq, const sources &src);

    void createGreens();
    void deleteGreens();

    void deletePtot();

    pressureFieldComplexSerial calcTotalField(const Greens_rect_2D_cpu &G, const pressureFieldSerial &chiEst, const pressureFieldComplexSerial &Pinit);

    void applyKappa(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double>* pData);
    void createKappa(const frequenciesGroup &freq, const sources &src, const receivers &recv);
    void deleteKappa();
};
