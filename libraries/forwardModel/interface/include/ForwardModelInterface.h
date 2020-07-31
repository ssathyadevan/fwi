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
            virtual std::vector<std::complex<double>> calculatePData(const core::dataGrid2D &chiEst) = 0;
            virtual void calculatePTot(const core::dataGrid2D &chiEst) = 0;
            virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator) = 0;

            virtual void calculateKappa() = 0;
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, core::complexDataGrid2D &) = 0;
            virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &, core::complexDataGrid2D &, const int, const int) = 0;
        };
    }   // namespace forwardModels
}   // namespace fwi
