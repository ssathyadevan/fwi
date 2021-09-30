#include "FiniteDifferenceOpenMPForwardModel.h"
#include "Helmholtz2D.h"
#include <complex>
#include <omp.h>
#include <vector>

namespace fwi
{
    namespace forwardModels
    {
        FiniteDifferenceOpenMPForwardModel::FiniteDifferenceOpenMPForwardModel(const core::grid2D &grid, const core::Sources &source,
            const core::Receivers &receiver, const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput)
            : FiniteDifferenceForwardModel(grid, source, receiver, freq, fmInput)
        {
            L_(io::linfo) << "OpenMP number of threads: " << omp_get_max_threads();
        }

        FiniteDifferenceOpenMPForwardModel::~FiniteDifferenceOpenMPForwardModel() {}

        void FiniteDifferenceOpenMPForwardModel::calculateKappa()
        {
            int li, lj, k;

#pragma omp parallel for private(li, lj, k)
            for(int i = 0; i < _freq.count; i++)
            {
                li = i * _receiver.count * _source.count;

                for(int j = 0; j < _receiver.count; j++)
                {
                    lj = j * _source.count;

                    for(k = 0; k < _source.count; k++)
                    {
                        _vkappa[li + lj + k] = (*_Greens[i].getReceiverCont(j)) * (_vpTot[i * _source.count + k]);
                    }
                }
            }
        }

        void FiniteDifferenceOpenMPForwardModel::calculatePTot(const core::dataGrid2D<double> &chiEst)
        {
            auto copyPTot = _vpTot;

            int li, i, j;
#pragma omp parallel for private(li, i, j)
            for(i = 0; i < _freq.count; i++)
            {
                li = i * _source.count;

                Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _source, _freq.c0, chiEst, _fMInput);

                L_(io::linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.count << "freq";

                for(j = 0; j < _source.count; j++)
                {
                    // Review: Remove logging statements?
                    // L_(io::linfo) << "Solving P_tot for source: (" << _source.xSrc[j][0] << "," << _source.xSrc[j][1] << ")";

                    _vpTot[li + j] = helmholtzFreq.solve(_source.xSrc[j], copyPTot[li + j]);
                }
            }
        }

        std::vector<std::complex<double>> FiniteDifferenceOpenMPForwardModel::calculatePressureField(const core::dataGrid2D<double> &chiEst)
        {
            std::vector<std::complex<double>> kOperator(_freq.count * _source.count * _receiver.count);
#pragma omp parallel for
            for(int i = 0; i < _freq.count * _source.count * _receiver.count; i++)
            {
                kOperator[i] = dotProduct(_vkappa[i], chiEst);
            }
            return kOperator;
        }

    }   // namespace forwardModels

}   // namespace fwi
