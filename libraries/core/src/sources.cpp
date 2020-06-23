#include "sources.h"
#include "log.h"

namespace fwi
{
    namespace core
    {
        sources::sources(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int nSrc_)
            : nSrc(nSrc_)
            , xSrc()
        {
            assert(nSrc > 1);

            std::array<double, 2> dx = calculateDistance(xMin, xMax);

            for(int i = 0; i < nSrc; i++)
            {
                std::array<double, 2> src;
                for(int j = 0; j < 2; j++)
                {
                    src[j] = xMin[j] + i * dx[j];
                }
                xSrc.push_back(src);
            }
        }

        std::array<double, 2> sources::calculateDistance(const std::array<double, 2> xMin, const std::array<double, 2> xMax)
        {
            std::array<double, 2> dx;
            for(int j = 0; j < 2; j++)
            {
                dx[j] = (xMax[j] - xMin[j]) / (nSrc - 1);
            }
            return dx;
        }

        void sources::Print()
        {
            L_(io::linfo) << "Total number of sources is " << nSrc << ". Positions:";
            for(int i = 0; i < nSrc; i++)
            {
                L_(io::linfo) << "x = (" << xSrc[i][0] << ", " << xSrc[i][1] << ")";
            }
        }
    }   // namespace core
}   // namespace fwi
