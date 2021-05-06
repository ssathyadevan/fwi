#ifndef FINITEDIFFERENCEFORWARDMODELPARALLEL_H
#define FINITEDIFFERENCEFORWARDMODELPARALLEL_H

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
            void calculateKappa();
            void calculatePTot(const core::dataGrid2D &chiEst);
        };
    }   // namespace forwardModels

}   // namespace fwi

#endif   // FINITEDIFFERENCEFORWARDMODELPARALLEL_H
