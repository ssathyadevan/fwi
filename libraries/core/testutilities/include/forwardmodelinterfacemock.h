#pragma once

#include "forwardModelInterface.h"
#include "freqInfo.h"
#include "frequenciesGroup.h"
#include "greensFunctions.h"
#include "greensSerial.h"

class ForwardModelInterfaceMock : public forwardModelInterface
{
    double _kappaTimesResidualValue;

public:
    ForwardModelInterfaceMock(const grid2D &grid, const sources &src, const receivers &recv, const frequenciesGroup &freq);

    ~ForwardModelInterfaceMock() {}
    void calculateKappa() {}

    virtual void calculatePData(const dataGrid2D &chiEst, std::vector<std::complex<double>> &pData);
    virtual void calculatePTot(const dataGrid2D &chiEst) { (void)chiEst; }
    virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, complexDataGrid2D &kappaTimesResidual)
    {
        kappaTimesResidual = _kappaTimesResidualValue;
    }   // the real KtimesResidual method cannot be applied without defining the whole complexDataGrid2D** _Kappa member, which defies the intent of creating a
        // mock class.
    virtual void mapDomainToSignal(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
    {
        (void)CurrentPressureFieldSerial;
        (void)kOperator;
    }
    // to change value to kappaTimesResidual. Can only be invoked when creating a ForwardModelInterfaceMock* and not a forwardModelInterface*
    void setKappaTimesResidualValue(double newKTRValue) { _kappaTimesResidualValue = newKTRValue; }
};
