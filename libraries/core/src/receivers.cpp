#include "receivers.h"

receivers::receivers(const sources &src) :
    nRecv(src.nSrc), xRecv(src.xSrc) {}

void receivers::Print()
{
    std::cout << "Total number is receivers is " << nRecv << ". Positions:" << std::endl;
    for(int i=0; i<nRecv; i++)
    {
        std::cout << "x = (" << xRecv[i][0] << ", " << xRecv[i][1] << ")" << std::endl;
    }
    std::cout << std::endl;
}



