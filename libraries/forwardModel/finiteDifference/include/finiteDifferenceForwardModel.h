#pragma once

#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"
#include "finiteDifferenceForwardModelInput.h"
#include "genericInput.h"

class finiteDifferenceForwardModel : public forwardModelInterface
{

public:
    finiteDifferenceForwardModel( const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput );

    ~finiteDifferenceForwardModel();

    virtual void calculatePData(const dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator);

    void calculateKappa();
    virtual void calculatePTot(const dataGrid2D &chiEst);
    virtual void getUpdateDirectionInformation(const std::vector<std::complex<double> > &res, complexDataGrid2D &kRes);
    virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &res, complexDataGrid2D &kRes, const int offset, const int block_size);
    virtual void getResidualGradient(std::vector<std::complex<double>> &res, complexDataGrid2D &kRes);
    virtual void mapDomainToSignal(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator);


    //void createKappaOperator(const complexDataGrid2D &CurrentPressureFieldComplexSerial, std::complex<double> *kOperator);

private:
    greensRect2DCpu **_Greens;

    complexDataGrid2D ***_p0;
    complexDataGrid2D **_pTot;
    complexDataGrid2D **_Kappa;
    finiteDifferenceForwardModelInput _fmInput;

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
