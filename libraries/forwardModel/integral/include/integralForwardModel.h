#pragma once

#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"
#include "integralForwardModelInput.h"
#include "genericInput.h"

class IntegralForwardModel : public forwardModelInterface
{

public:

    IntegralForwardModel( const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq, const integralForwardModelInput &fmInput );

    ~IntegralForwardModel();

    virtual void calculatePData(const pressureFieldSerial &chiEst, std::vector<std::complex<double>> &kOperator);

    void calculateKappa();
    virtual void calculatePTot(const pressureFieldSerial &chiEst);
    virtual void getUpdateDirectionInformation(std::vector<std::complex<double>> &res, pressureFieldComplexSerial &kRes);
    virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &res, pressureFieldComplexSerial &kRes, const int offset, const int block_size);

    virtual void mapDomainToSignal(const pressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator);

private:
    greensRect2DCpu **_Greens;

    pressureFieldComplexSerial ***_p0;
    pressureFieldComplexSerial **_pTot;
    pressureFieldComplexSerial **_Kappa;
    integralForwardModelInput _fmInput;

    void createP0();
    void deleteP0();

    void createPTot(const frequenciesGroup &freq, const sources &src);

    void createGreens();
    void deleteGreens();

    void deletePtot();

    pressureFieldComplexSerial calcTotalField(const greensRect2DCpu &G, const pressureFieldSerial &chiEst, const pressureFieldComplexSerial &Pinit);

    void applyKappa(const pressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &pData);
    void createKappa(const frequenciesGroup &freq, const sources &src, const receivers &recv);
    void deleteKappa();
};
