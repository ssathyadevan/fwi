#include "FiniteDifferenceForwardModelParallelMPI.h"
#include "Helmholtz2D.h"
#include <complex>
#include <mpi.h>
#include <vector>

namespace fwi
{
    namespace forwardModels
    {
        FiniteDifferenceForwardModelParallelMPI::FiniteDifferenceForwardModelParallelMPI(const core::grid2D &grid, const core::Sources &source,
            const core::Receivers &receiver, const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput)
            : FiniteDifferenceForwardModel(grid, source, receiver, freq, fmInput)
        {
        }

        void FiniteDifferenceForwardModelParallelMPI::calculatePTot(const core::dataGrid2D &chiEst)
        {
            assert(_Greens != nullptr);
            assert(_p0 != nullptr);

            auto copyPTot = _pTot;

            int rank, size;
            MPI_Comm_rank(MPI_COMM_WORLD, &rank); /* get current process id */
            MPI_Comm_size(MPI_COMM_WORLD, &size); /* get number of processes */
            printf("Hello world from process %d of %d\n", rank, size);
            // calculate the size
            int total_loop_size = _freq.count;
            int sep_loop_size = total_loop_size / size;
            int loop_size = sep_loop_size;
            // This
            if(rank == 0)
            {
                loop_size = sep_loop_size + (total_loop_size - sep_loop_size * size);
            }
            else
            {
                loop_size = sep_loop_size;
            }
            int start = rank * loop_size;
            int stop = (rank + 1) * loop_size;

            int li, i, j;
            for(i = start; i < stop; i++)
            {
                li = i * _source.count;

                Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _source, _freq.c0, chiEst, _fMInput);

                L_(io::linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.count << "freq. For rank:" << rank;

                for(j = 0; j < _source.count; j++)
                {
                    // L_(io::linfo) << "Solving P_tot for source: (" << _source.xSrc[j][0] << "," << _source.xSrc[j][1] << ")";

                    *_pTot[li + j] = helmholtzFreq.solve(_source.xSrc[j], *copyPTot[li + j]);
                }
            }
            MPI_Finalize();
        }

    }   // namespace forwardModels

}   // namespace fwi
