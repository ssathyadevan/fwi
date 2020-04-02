#pragma once

#include "forwardModelInterface.h"
#include "genericInput.h"
#include "greensFunctions.h"
#include "greensSerial.h"

class ForwardModelInterfaceMock : public forwardModelInterface
{
public:
    ForwardModelInterfaceMock(const grid2D &grid, const sources &src, const receivers &recv, const frequenciesGroup &freq);

    ~ForwardModelInterfaceMock() {}
    void calculateKappa() {}

    virtual void calculatePData(const pressureFieldSerial &chiEst, std::vector<std::complex<double>> &pData);
    virtual void calculatePTot(const pressureFieldSerial &chiEst) { (void)chiEst; }
    virtual void mapDomainToSignal(const pressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
    {
        (void)CurrentPressureFieldSerial;
        (void)kOperator;
    }
};
