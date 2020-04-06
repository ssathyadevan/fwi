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

    virtual void calculatePData(const dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator);

    void calculateKappa();
    virtual void calculatePTot(const dataGrid2D &chiEst);
    virtual void getUpdateDirectionInformation(std::vector<std::complex<double>> &res, complexDataGrid2D &kRes);
    virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &res, complexDataGrid2D &kRes, const int offset, const int block_size);

    virtual void mapDomainToSignal(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator);

private:
    greensRect2DCpu **_Greens;

    complexDataGrid2D ***_p0;
    complexDataGrid2D **_pTot;
    complexDataGrid2D **_Kappa;
    integralForwardModelInput _fmInput;

    void createP0();
    void deleteP0();

    void createPTot(const frequenciesGroup &freq, const sources &src);

    void createGreens();
    void deleteGreens();

    void deletePtot();

    complexDataGrid2D calcTotalField(const greensRect2DCpu &G, const dataGrid2D &chiEst, const complexDataGrid2D &Pinit);

    void applyKappa(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &pData);
    void createKappa(const frequenciesGroup &freq, const sources &src, const receivers &recv);
    void deleteKappa();
};
