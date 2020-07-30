#pragma once

#include "ForwardModelInterface.h"
#include "genericInput.h"
#include "greensFunctions.h"
#include "greensSerial.h"
#include "integralForwardModelInput.h"

namespace fwi
{
    namespace forwardModels
    {
        class IntegralForwardModel : public ForwardModelInterface
        {
        public:
            IntegralForwardModel(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver, const core::FrequenciesGroup &freq,
                const integralForwardModelInput &fmInput);

            ~IntegralForwardModel();

            virtual void calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator);

            virtual void calculatePTot(const core::dataGrid2D &chiEst);
            void calculateKappa();
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes);
            virtual void getUpdateDirectionInformationMPI(
                std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes, const int offset, const int block_size);

            virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator);

            const core::grid2D &getGrid() { return _grid; }

            const core::Sources &getSource() { return _source; }

            const core::Receivers &getReceiver() { return _receiver; }

            const core::FrequenciesGroup &getFreq() { return _freq; }

        private:
            std::vector<std::complex<double>> _residual;

            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;

            CostFunction _costFunction;

            core::greensRect2DCpu **_Greens;

            core::complexDataGrid2D ***_p0;
            core::complexDataGrid2D **_pTot;
            core::complexDataGrid2D **_Kappa;
            integralForwardModelInput _fmInput;

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
        };

    }   // namespace forwardModels
}   // namespace fwi
