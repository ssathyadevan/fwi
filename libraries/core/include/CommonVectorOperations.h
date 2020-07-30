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
        inline double l2NormSquared(const std::vector<std::complex<double>> &data)
        {
            double initialValue = 0.0;
            auto elementOperation = [](double accumulatedValue, std::complex<double> vectorElement) { return (accumulatedValue + std::norm(vectorElement)); };
            return std::accumulate(data.begin(), data.end(), initialValue, elementOperation);
        }

        template<class type_t> inline std::vector<type_t> operator-(const std::vector<type_t> &operandA, const std::vector<type_t> &operandB)
        {
            assert(operandA.size() == operandB.size() && "Substraction is not defined for vectors of different size");
            std::vector<type_t> result(operandA.size());
            std::transform(operandA.begin(), operandA.end(), operandB.begin(), result.begin(), [](type_t x, type_t y) { return x - y; });
            return result;
        }

        template<class type_t> inline std::vector<type_t> operator+(const std::vector<type_t> &operandA, const std::vector<type_t> &operandB)
        {
            assert(operandA.size() == operandB.size() && "Addition is not defined for vectors of different size");
            std::vector<type_t> result(operandA.size());
            std::transform(operandA.begin(), operandA.end(), operandB.begin(), result.begin(), [](type_t x, type_t y) { return x + y; });
            return result;
        }

    }   // namespace core
}   // namespace fwi
