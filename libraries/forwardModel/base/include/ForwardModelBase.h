#pragma once
#include "forwardModelInterface.h"
#include "log.h"

namespace fwi
{
    namespace forwardModels
    {
        class ForwardModelBase: public forwardModelInterface
        {

        public:
            ForwardModelBase(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver, const core::FrequenciesGroup &freq);

            virtual ~ForwardModelBase();

            const core::grid2D &getGrid();
            const core::Sources &getSource();
            const core::Receivers &getReceiver();
            const core::FrequenciesGroup &getFreq();
            CostFunction getCostFunction();

            virtual void calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &pData) = 0;
            virtual void calculatePTot(const core::dataGrid2D &chiEst) = 0;
            virtual void mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator) = 0;

             virtual void calculateKappa()
            {
                L_(io::lerror) << "This ForwardModel is not compatible with the Inversion model";
                exit(EXIT_FAILURE);
            }
            virtual void getUpdateDirectionInformation(const std::vector<std::complex<double>> &, core::complexDataGrid2D &)
            {
                L_(io::lerror) << "This ForwardModel is not compatible with the Inversion model";
                exit(EXIT_FAILURE);
            }
            virtual void getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &, core::complexDataGrid2D &, const int, const int)
            {
                L_(io::lerror) << "This ForwardModel is not compatible with the Inversion model";
                exit(EXIT_FAILURE);
            }
            std::vector<std::complex<double>> &calculateResidual(const core::dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef);
            double calculateResidualNormSq(const std::vector<std::complex<double>> &residual);
            double calculateCost(
                std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
            double calculateLeastSquaresCost(
                std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta);
        private:
            std::vector<std::complex<double>> _residual;

            const core::grid2D &_grid;
            const core::Sources &_source;
            const core::Receivers &_receiver;
            const core::FrequenciesGroup &_freq;

            CostFunction _costFunction;

        };
    }
}
