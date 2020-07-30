#pragma once

#include <algorithm>
#include <cassert>
#include <complex>
#include <numeric>
#include <vector>

namespace fwi
{
    namespace core
    {
        inline double l2NormSq(const std::vector<std::complex<double>> &data)
        {
            double initialValue = 0.0;
            auto elementOperation = [](double accumulatedValue, std::complex<double> vectorElement) { return (accumulatedValue + std::norm(vectorElement)); };
            return std::accumulate(data.begin(), data.end(), initialValue, elementOperation);
        }

        inline std::vector<std::complex<double>> operator-(const std::vector<std::complex<double>> &operandA, const std::vector<std::complex<double>> &operandB)
        {
            assert(operandA.size() == operandB.size() && "Substraction is not allowed in vectors of different size");
            std::vector<std::complex<double>> result(operandA.size());
            auto operationBetweenElements = [](std::complex<double> x, std::complex<double> y) { return x - y; };
            std::transform(operandA.begin(), operandA.end(), operandB.begin(), result.begin(), operationBetweenElements);
            return result;
        }
    }   // namespace core
}   // namespace fwi
