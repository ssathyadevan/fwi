#pragma once

#include "dataGrid2D.h"
#include <complex>

namespace fwi
{
    namespace core
    {
        inline double normSq(const std::vector<std::complex<double>> &data)
        {
            double result = 0.0;
            for(size_t i = 0; i < data.size(); i++)
            {
                result += std::norm(data[i]);
            }

            return result;
        }

        class CostFunctionCalculator
        {
            const int _costFunction;

        public:
            CostFunctionCalculator(int costFunctionSelected = 0)
                : _costFunction(costFunctionSelected)
            {
            }

            double calculateCost(const std::vector<std::complex<double>> &pData, const std::vector<std::complex<double>> &pDataEst, double eta)
            {
                /*
                 *         double ForwardModelBase::calculateCost(
            std::vector<std::complex<double>> &residualArray, core::dataGrid2D &chiEstimate, const std::vector<std::complex<double>> &pData, double eta)
                        switch(_costFunction)
                        {
                            case leastSquares: cost = calculateLeastSquaresCost(residualArray, chiEstimate, pData, eta); break;
                            default: throw std::invalid_argument("Invalid cost function selected.");
                        }
                */
                return eta * normSq(calculateResidual(pData, pDataEst));
            }

            std::vector<std::complex<double>> calculateResidual(
                const std::vector<std::complex<double>> &pDataRef, const std::vector<std::complex<double>> &pDataEst) const
            {
                /*
                     std::vector<std::complex<double>> &ForwardModelBase::calculateResidual(
                         const core::dataGrid2D &chiEst, const std::vector<std::complex<double>> &pDataRef)
                       {  std::vector<std::complex<double>> pDataEst(pDataRef.size());

                         calculatePData(chiEst, pDataEst);
                   if(isnan(std::abs(residual[i])))
                    {
                        throw std::overflow_error("Residual is not a number");
                    }}
                */

                std::vector<std::complex<double>> residual;
                residual.resize(pDataRef.size());
                for(size_t i = 0; i < pDataRef.size(); i++)
                {
                    residual[i] = pDataRef[i] - pDataEst[i];
                }

                return residual;
            }
        };
    }   // namespace core
}   // namespace fwi
/*
class CostFunctionCalculator
{
    const int _costFunction;

    public:
    CostFunctionCalculator(int costFunctionSelected = 0):_costFunctionSelected(costFunctionSelected){}
    double calculateCost() const;
};*/
