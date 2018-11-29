#include "receivers_rect_2D.h"

Receivers_rect_2D::Receivers_rect_2D(const Sources_rect_2D &src) :
    nRecv(src.nSrc), xRecv(src.xSrc) {}

void Receivers_rect_2D::Print()
{
    std::cout << "Total number is receivers is " << nRecv << ". Positions:" << std::endl;
    for(int i=0; i<nRecv; i++)
    {
        std::cout << "x = (" << xRecv[i][0] << ", " << xRecv[i][1] << ")" << std::endl;
    }
    std::cout << std::endl;
}



