#include <gtest/gtest.h>
#include "forwardModelInterface.h"

TEST(forwardModelInterfaceTest, constructorTest)
{
    const Grid2D grid2D({1.0, 1.0}, {1.0, 1.0}, {1, 1});

    const Sources src({1.0, 1.0}, {1.0, 1.0}, 2);

    const Receivers recv({1.0, 1.0}, {1.0, 1.0}, 2);

    const Freq freq;

    FrequenciesGroup(freq, 1.0);

    ForwardModelInterface forwardModelInterface(grid2D, src, recv, freq);
}

ForwardModelInterface::ForwardModelInterface(const Grid2D &grid, const Sources &src, const Receivers &recv,
                                             const FrequenciesGroup &freq)
    : /*_residual(), */ _grid(grid), _src(src), _recv(recv), _freq(freq)
{
    _residual = std::vector<std::complex<double>>(_freq.nFreq * _src.nSrc * _recv.nRecv);
}