#include "receivers.h"
#include "log.h"

Receivers::Receivers(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int nRecv_)
    : nRecv(nRecv_), xRecv()
{
    assert(nRecv > 1);

    std::array<double, 2> dx = calculate_distance(xMin, xMax);

    for (int i = 0; i < nRecv; i++)
    {
        std::array<double, 2> recv;
        for (int j = 0; j < 2; j++)
        {
            recv[j] = xMin[j] + i * dx[j];
        }
        xRecv.push_back(recv);
    }
}

std::array<double, 2> Receivers::calculate_distance(const std::array<double, 2> xMin, const std::array<double, 2> xMax)
{
    std::array<double, 2> dx;
    for (int j = 0; j < 2; j++)
    {
        dx[j] = (xMax[j] - xMin[j]) / (nRecv - 1);
    }
    return dx;
}

void Receivers::Print()
{
    L_(linfo) << "Total number is receivers is " << nRecv << ". Positions:" ;
    for (int i = 0; i < nRecv; i++)
    {
        L_(linfo) << "x = (" << xRecv[i][0] << ", " << xRecv[i][1] << ")" ;
    }
}
