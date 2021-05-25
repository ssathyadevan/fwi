#ifndef FINITEDIFFERENCEFORWARDMODELPARALLELMPI_H
#define FINITEDIFFERENCEFORWARDMODELPARALLELMPI_H

#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInput.h"
#include "ForwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"

namespace fwi
{
    namespace forwardModels
    {
        class FiniteDifferenceForwardModelParallelMPI : public FiniteDifferenceForwardModel
        {
        public:
            FiniteDifferenceForwardModelParallelMPI(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
                const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput);
            // Overriding ForwardModel
            // std::vector<std::complex<double>> calculatePressureField(const core::dataGrid2D &chiEst);
            // void calculateKappa();
            void calculatePTot(const core::dataGrid2D &chiEst);

            // void getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
        };
    }   // namespace forwardModels

}   // namespace fwi

#endif   // FINITEDIFFERENCEFORWARDMODELPARALLELMPI_H
