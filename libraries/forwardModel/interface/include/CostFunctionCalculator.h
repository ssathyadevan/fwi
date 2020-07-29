#pragma once

#include "commonVectorOperations.h"

namespace fwi
{
    namespace core
    {
        class CostFunctionCalculator
        {
        public:
            enum CostFunction
            {
                leastSquares
            };

            CostFunctionCalculator(CostFunction costFunctionSelected = leastSquares)
                : _costFunction(costFunctionSelected)
            {
            }

            double calculateCost(const std::vector<std::complex<double>> &pData, const std::vector<std::complex<double>> &pDataEst, double eta) const
            {
                return eta * l2NormSq(pData - pDataEst);
            }

        private:
            const CostFunction _costFunction;
        };
    }   // namespace core
}   // namespace fwi
