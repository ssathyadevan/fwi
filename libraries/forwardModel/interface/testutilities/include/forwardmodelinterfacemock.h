#pragma once

#include "forwardModelInterface.h"
#include "freqInfo.h"
#include "frequenciesGroup.h"
#include "greensFunctions.h"
#include "greensSerial.h"

namespace forwardModels
{
    class ForwardModelInterfaceMock : public forwardModelInterface
    {
        double _kappaTimesResidualValue;

    public:
        ForwardModelInterfaceMock(const core::grid2D &grid, const core::sources &src, const core::receivers &recv, const core::frequenciesGroup &freq);

        ~ForwardModelInterfaceMock() {}
        void calculateKappa() {}

        virtual void calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &pData);
        virtual void calculatePTot(const core::dataGrid2D &chiEst) { (void)chiEst; }
        virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, core::complexDataGrid2D &kappaTimesResidual)
        {
            kappaTimesResidual = _kappaTimesResidualValue;
        }   // the real KtimesResidual method cannot be applied without defining the whole core::complexDataGrid2D** _Kappa member, which defies the intent of
            // creating a mock class.
        virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
        {
            (void)CurrentPressureFieldSerial;
            (void)kOperator;
        }
        // to change value to kappaTimesResidual. Can only be invoked when creating a ForwardModelInterfaceMock* and not a forwardModelInterface*
        void setKappaTimesResidualValue(double newKTRValue) { _kappaTimesResidualValue = newKTRValue; }
    };
}   // namespace forwardModels
