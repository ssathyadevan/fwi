#ifndef FINITEDIFFERENCEFORWARDMODELPARALLEL_H
#define FINITEDIFFERENCEFORWARDMODELPARALLEL_H
// Review: no pragma once or is this imported by the non paralell header files?
#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInput.h"
#include "ForwardModelInterface.h"
#include "greensFunctions.h"
#include "greensSerial.h"

namespace fwi
{
    namespace forwardModels
    {
        class FiniteDifferenceForwardModelParallel : public FiniteDifferenceForwardModel
        {
        public:
            FiniteDifferenceForwardModelParallel(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
                const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput);
            // Overriding ForwardModel
            std::vector<std::complex<double>> calculatePressureField(const core::dataGrid2D &chiEst);
            void calculateKappa();
            void calculatePTot(const core::dataGrid2D &chiEst);
            void getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
        };
    }   // namespace forwardModels

}   // namespace fwi

#endif   // FINITEDIFFERENCEFORWARDMODELPARALLEL_H
