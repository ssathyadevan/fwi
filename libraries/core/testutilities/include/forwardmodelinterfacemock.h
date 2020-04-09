#pragma once

#include "forwardModelInterface.h"
#include "freqInfo.h"
#include "frequenciesGroup.h"
#include "genericInput.h"
#include "greensFunctions.h"
#include "greensSerial.h"

class ForwardModelInterfaceMock : public forwardModelInterface
{
public:
    ForwardModelInterfaceMock(const grid2D &grid, const sources &src, const receivers &recv, const frequenciesGroup &freq);

    ~ForwardModelInterfaceMock() {}
    void calculateKappa() {}

    virtual void calculatePData(const dataGrid2D &chiEst, std::vector<std::complex<double>> &pData);
    virtual void calculatePTot(const dataGrid2D &chiEst) { (void)chiEst; }
    virtual void mapDomainToSignal(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
    {
        (void)CurrentPressureFieldSerial;
        (void)kOperator;
    }
};
