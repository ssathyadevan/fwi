#pragma once

#include "CommonVectorOperations.h"

namespace fwi
{
    namespace core
    {
        class CostFunctionCalculator
        {
        public:
            enum class CostFunctionEnum
            {
                leastSquares
            };

            explicit CostFunctionCalculator(CostFunctionEnum costFunctionSelected = CostFunctionEnum::leastSquares)
                : _costFunction(costFunctionSelected)
            {
            }

            double calculateCost(const std::vector<std::complex<double>> &pData, const std::vector<std::complex<double>> &pDataEst, double eta) const
            {
                return eta * l2NormSq(pData - pDataEst);
            }

            CostFunctionEnum getCostFunction() const { return _costFunction; }

        private:
            const CostFunctionEnum _costFunction;
        };
    }   // namespace core
}   // namespace fwi
