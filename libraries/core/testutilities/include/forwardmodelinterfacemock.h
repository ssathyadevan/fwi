#pragma once

#include "forwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"
#include "forwardModelInterface.h"
#include "genericInput.h"

class ForwardModelInterfaceMock : public ForwardModelInterface
{

public:
    ForwardModelInterfaceMock( const Grid2D &grid, const Sources &src, const Receivers &recv,
                               const FrequenciesGroup &freq);

    ~ForwardModelInterfaceMock(){}
    void calculateKappa(){}

    virtual void calculatePData(const PressureFieldSerial &chiEst, std::vector<std::complex<double>> &pData);
    virtual void calculatePTot(const PressureFieldSerial &chiEst){chiEst.GetGrid();}
    virtual void mapDomainToSignal(const PressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator){CurrentPressureFieldSerial.GetDataPtr(); kOperator.end();}
};
