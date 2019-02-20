#include "forwardModelInterface.h"


const grid2D& ForwardModelInterface::getGrid()
{
    return grid;
}

const sources& ForwardModelInterface::getSrc()
{
    return src;
}

const receivers& ForwardModelInterface::getRecv()
{
    return recv;
}

const frequenciesGroup& ForwardModelInterface::getFreq()
{
    return freq;
}
