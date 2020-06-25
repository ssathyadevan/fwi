#pragma once

#include <array>
#include <iostream>
#include <vector>

#include "sources.h"

namespace fwi
{
    namespace core
    {
        class receivers
        {
            receivers &operator=(const receivers &) = delete;

        public:
            const int nRecv;
            std::vector<std::array<double, 2>> xRecv;

            receivers(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int nRecv_);

            void Print();

        private:
            std::array<double, 2> calculateDistance(const std::array<double, 2> xMin, const std::array<double, 2> xMax);
        };
    }   // namespace core
}   // namespace fwi
