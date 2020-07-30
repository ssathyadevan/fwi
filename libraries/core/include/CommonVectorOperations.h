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
        /**
         * @brief Computes the l2 norm squared of a std::vector<std::complex<double>>
         * @param data is a reference to the vector to use to calculate its l2 norm squared
         * @return calculated l2 norm squared
         */
        inline double l2NormSquared(const std::vector<std::complex<double>> &data)
        {
            double initialValue = 0.0;
            auto elementOperation = [](double accumulatedValue, std::complex<double> vectorElement) { return (accumulatedValue + std::norm(vectorElement)); };
            return std::accumulate(data.begin(), data.end(), initialValue, elementOperation);
        }

        /**
         * @brief Defines substraction operator between vectors of equal size, it performs element wise substraction
         * @param operandA constant reference to the first operand
         * @param operandB constant reference to the second operand
         * @return a vector where its components are calculated as vector[i] = operandA[i] - operandB[i]
         */
        template<class type_t> inline std::vector<type_t> operator-(const std::vector<type_t> &operandA, const std::vector<type_t> &operandB)
        {
            assert(operandA.size() == operandB.size() && "Substraction is not defined for vectors of different size");
            std::vector<type_t> result(operandA.size());
            std::transform(operandA.begin(), operandA.end(), operandB.begin(), result.begin(), [](type_t x, type_t y) { return x - y; });
            return result;
        }

        /**
         * @brief Defines addition operator between vectors of equal size, it performs element wise addition
         * @param operandA constant reference to the first operand
         * @param operandB constant reference to the second operand
         * @return a vector where its components are calculated as vector[i] = operandA[i] + operandB[i]
         */
        template<class type_t> inline std::vector<type_t> operator+(const std::vector<type_t> &operandA, const std::vector<type_t> &operandB)
        {
            assert(operandA.size() == operandB.size() && "Addition is not defined for vectors of different size");
            std::vector<type_t> result(operandA.size());
            std::transform(operandA.begin(), operandA.end(), operandB.begin(), result.begin(), [](type_t x, type_t y) { return x + y; });
            return result;
        }

    }   // namespace core
}   // namespace fwi
