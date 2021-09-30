#ifdef MPI
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
        class FiniteDifferenceForwardModelMPI : public FiniteDifferenceForwardModel
        {
        public:
            FiniteDifferenceForwardModelMPI(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
                const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput);
            ~FiniteDifferenceForwardModelMPI();
        };

    }   // namespace forwardModels
}   // namespace fwi
#endif
