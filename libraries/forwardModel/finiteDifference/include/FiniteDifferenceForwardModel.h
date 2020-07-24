#pragma once

#include "FiniteDifferenceForwardModelInput.h"
#include "ForwardModelBase.h"
#include "greensFunctions.h"
#include "greensSerial.h"

namespace fwi
{
    namespace forwardModels
    {
        class FiniteDifferenceForwardModel : public ForwardModelBase
        {
        public:
            FiniteDifferenceForwardModel(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
                const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput);

            ~FiniteDifferenceForwardModel();

            virtual void calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator);

            void calculateKappa();
            virtual void calculatePTot(const core::dataGrid2D &chiEst);
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
            virtual void getUpdateDirectionInformationMPI(
                std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes, const int offset, const int block_size);
            virtual void getResidualGradient(std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
            virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator);

        private:


            void createP0();
            void deleteP0();

            void createPTot(const core::FrequenciesGroup &freq, const core::Sources &source);

            void createGreens();
            void deleteGreens();

            void deletePtot();

            core::complexDataGrid2D calcTotalField(const core::greensRect2DCpu &G, const core::dataGrid2D &chiEst, const core::complexDataGrid2D &Pinit);

            void applyKappa(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &pData);
            void createKappa(const core::FrequenciesGroup &freq, const core::Sources &source, const core::Receivers &receiver);
            void deleteKappa();
            void configureCostFunction(CostFunction costFunction);

        private:
            std::vector<std::complex<double>> _residual;
            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;
            core::greensRect2DCpu **_Greens;

            core::complexDataGrid2D ***_p0;
            core::complexDataGrid2D **_pTot;
            core::complexDataGrid2D **_kappa;
            finiteDifferenceForwardModelInput _fMInput;

            CostFunction _costFunction;
        };

    }   // namespace forwardModels
}   // namespace fwi
