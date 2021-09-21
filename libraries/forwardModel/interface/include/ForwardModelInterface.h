#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"
#include "frequenciesGroup.h"
#include "grid2D.h"
#include "log.h"
#include "receivers.h"
#include "sources.h"
#include <complex>

namespace fwi
{
    namespace forwardModels
    {
        /**
         * @brief Interface for ForwardModel classes
         */
        template <class _V = double>
        class ForwardModelInterface
        {
        public:
            ForwardModelInterface() = default;

            virtual ~ForwardModelInterface() = default;
            ///@{
            /** Getter functions for ForwardModel initialization parameters */
            virtual const core::grid2D &getGrid() = 0;
            virtual const core::Sources &getSource() = 0;
            virtual const core::Receivers &getReceiver() = 0;
            virtual const core::FrequenciesGroup &getFreq() = 0;
            ///@}
            /**
             * @brief calculatePData Calculates pressure field from media properties
             * @param chiEst reference to media properties
             * @return pressure field
             */
            virtual std::vector<std::complex<double>> calculatePressureField(const core::complexDataGrid2D<_V> &chiEst) = 0;

            // TODO: Evaluate the following functions for removal
            virtual void calculatePTot(const core::complexDataGrid2D<_V> &chiEst) = 0;
            virtual void calculateKappa() = 0;

            // TODO: Since this method is only used in conjugate gradient inversion algorithms, should this actually be in this class?
            /**
             * @brief given a newly computed std::vector<std::complex<double>> residual and a core::complexDataGrid2D kappaTimesResidual, updates kappaTimesResidual with the new value of _Kappa multiplied by residual.
             * getUpdateDirectionInformation() is used only by the conjugate gradient inversion algorithms, see eq. (37) of /doc/ReadMe/1_ProjectDescription.pdf
             */
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, core::complexDataGrid2D<std::complex<double>> &) = 0;
            virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &, core::complexDataGrid2D<std::complex<double>> &, const int, const int) = 0;
        };
    }   // namespace forwardModels
}   // namespace fwi
