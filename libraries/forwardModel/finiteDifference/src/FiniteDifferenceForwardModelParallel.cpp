#include "FiniteDifferenceForwardModelParallel.h"
#include "Helmholtz2D.h"
#include <omp.h>

namespace fwi
{
    namespace forwardModels
    {
        FiniteDifferenceForwardModelParallel::FiniteDifferenceForwardModelParallel(const core::grid2D &grid, const core::Sources &source,
            const core::Receivers &receiver, const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput)
            : FiniteDifferenceForwardModel(grid, source, receiver, freq, fmInput)
        {
        }

        void FiniteDifferenceForwardModelParallel::calculateKappa()
        {
            int li, lj;

#pragma omp parallel for
            for(int i = 0; i < _freq.count; i++)
            {
                li = i * _receiver.count * _source.count;

                for(int j = 0; j < _receiver.count; j++)
                {
                    lj = j * _source.count;

                    for(int k = 0; k < _source.count; k++)
                    {
                        *_kappa[li + lj + k] = (*_Greens[i]->getReceiverCont(j)) * (*_pTot[i * _source.count + k]);
                    }
                }
            }
        }

        void FiniteDifferenceForwardModelParallel::calculatePTot(const core::dataGrid2D &chiEst)
        {
            assert(_Greens != nullptr);
            assert(_p0 != nullptr);

            int li;
#pragma omp parallel for
            for(int i = 0; i < _freq.count; i++)
            {
                li = i * _source.count;

                Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _source, _freq.c0, chiEst, _fMInput);

                L_(io::linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.count << "freq";

                for(int j = 0; j < _source.count; j++)
                {
                    L_(io::linfo) << "Solving p_tot for source: (" << _source.xSrc[j][0] << "," << _source.xSrc[j][1] << ")";
                    *_pTot[li + j] = helmholtzFreq.solve(_source.xSrc[j], *_pTot[li + j]);
                }
            }
        }

    }   // namespace forwardModels

}   // namespace fwi
