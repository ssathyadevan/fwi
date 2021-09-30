#include "conjugateGradientOpenMPinversion.h"
#include <omp.h>

namespace fwi
{
    namespace inversionMethods
    {
        conjugateGradientOpenMPinversion::conjugateGradientOpenMPinversion(const core::CostFunctionCalculator &costCalculator,
            forwardModels::ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput &invInput)
            : ConjugateGradientInversion(costCalculator, forwardModel, invInput)
        {
        }

        conjugateGradientOpenMPinversion::~conjugateGradientOpenMPinversion() {}

        void conjugateGradientOpenMPinversion::getUpdateDirectionInformation(
            const std::vector<std::complex<double>> &residualVector, core::dataGrid2D<std::complex<double>> &kappaTimesResidual)
        {
#pragma omp declare reduction(addition : class core::dataGrid2D <std::complex <double>> : omp_out += omp_in) initializer(omp_priv(omp_orig))
#pragma omp declare reduction(multiplication : class core::dataGrid2D <std::complex <double>> : omp_out *omp_in) initializer(omp_priv(omp_orig))

            int l_i, l_j;
            kappaTimesResidual.zero();
            core::dataGrid2D<std::complex<double>> kDummy(_grid);
            auto kappa = _forwardModel->getKernel();
#pragma omp parallel for reduction(addition : kappaTimesResidual) reduction(multiplication : kDummy) private(l_i, l_j)
            for(int i = 0; i < _frequencies.count; i++)
            {
                l_i = i * _receivers.count * _sources.count;
                for(int j = 0; j < _receivers.count; j++)
                {
                    l_j = j * _receivers.count;
                    for(int k = 0; k < _receivers.count; k++)
                    {
                        kDummy = kappa[l_i + l_j + k];
                        kDummy.conjugate();
                        kappaTimesResidual += kDummy * residualVector[l_i + l_j + k];
                    }
                }
            }
        }

    }   // namespace inversionMethods

}   // namespace fwi
