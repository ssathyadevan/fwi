#include "receivers.h"

// Place the receivers on a linear line from xMin to xMax, including the end points
receivers::receivers(const std::array<double, 2> xMin, const std::array<double, 2> xMax, int nRecv_)
    : nRecv(nRecv_), xRecv()
{
    assert(nRecv > 1);

    std::array<double, 2> dx;
    for (int j = 0; j < 2; j++)
    {
        dx[j] = (xMax[j] - xMin[j]) / (nRecv - 1);
    }

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

void receivers::Print()
{
    std::cout << "Total number is receivers is " << nRecv << ". Positions:" << std::endl;
    for (int i = 0; i < nRecv; i++)
    {
        std::cout << "x = (" << xRecv[i][0] << ", " << xRecv[i][1] << ")" << std::endl;
    }
    std::cout << std::endl;
}
