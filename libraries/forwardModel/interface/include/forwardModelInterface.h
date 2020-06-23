#pragma once

#include "complexDataGrid2D.h"
#include "dataGrid2D.h"
#include "frequenciesGroup.h"
#include "grid2D.h"
#include "log.h"
#include "receivers.h"
#include "sources.h"
#include <complex>

namespace forwardModels
{
    inline double normSq(const std::vector<std::complex<double>> &data, int n)
    {
        double result = double(0.0);
        for(int i = 0; i < n; i++)
        {
            result += std::norm(data[i]);
        }

        return result;
    }

    class forwardModelInterface
    {
    public:
        forwardModelInterface(const core::grid2D &grid, const core::sources &src, const core::receivers &recv, const core::frequenciesGroup &freq);

        virtual ~forwardModelInterface();

        const core::grid2D &getGrid();
        const core::sources &getSrc();
        const core::receivers &getRecv();
        const core::frequenciesGroup &getFreq();

        enum CostFunction
        {
            leastSquares
        };
        CostFunction costFunction;

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

    protected:
        const core::grid2D &_grid;
        const core::sources &_src;
        const core::receivers &_recv;
        const core::frequenciesGroup &_freq;
    };
}   // namespace forwardModels
