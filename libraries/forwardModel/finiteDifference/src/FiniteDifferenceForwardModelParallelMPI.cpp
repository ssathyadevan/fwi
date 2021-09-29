#ifdef MPI

#include "FiniteDifferenceForwardModelParallelMPI.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "Helmholtz2D.h"
#include "log.h"
#include <boost/mpi.hpp>
#include <boost/serialization/complex.hpp>
#include <boost/serialization/vector.hpp>
namespace mpi = boost::mpi;

namespace fwi
{
    namespace forwardModels
    {
        FiniteDifferenceForwardModelMPI::FiniteDifferenceForwardModelMPI(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
            const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fMInput)
            : FiniteDifferenceForwardModel(grid, source, receiver, freq, fMInput)
        {
        }

        FiniteDifferenceForwardModelMPI::~FiniteDifferenceForwardModelMPI() {}

    }   // namespace forwardModels
}   // namespace fwi
#endif
