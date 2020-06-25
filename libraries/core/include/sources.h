#pragma once

#include <array>
#include <cassert>
#include <iostream>
#include <vector>

namespace fwi
{
    namespace core
    {
        class sources
        {
        public:
            int count;
            std::vector<std::array<double, 2>> xSrc;
            // Place the sources on a linear line from xMin to xMax, including the end points
            sources(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int count_);

            void Print();

        private:
            std::array<double, 2> calculateDistance(const std::array<double, 2> xMin, const std::array<double, 2> xMax);
        };
    }   // namespace core
}   // namespace fwi
