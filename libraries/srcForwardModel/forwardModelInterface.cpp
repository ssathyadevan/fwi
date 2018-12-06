#include "forwardModelInterface.h"


const grid_rect_2D& ForwardModelInterface::getGrid()
{
    return grid;
}

const Sources_rect_2D& ForwardModelInterface::getSrc()
{
    return src;
}

const Receivers_rect_2D& ForwardModelInterface::getRecv()
{
    return recv;
}

const Frequencies_group& ForwardModelInterface::getFreq()
{
    return freq;
}
