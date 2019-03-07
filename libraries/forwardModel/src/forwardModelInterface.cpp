#include "forwardModelInterface.h"


const grid2D& ForwardModelInterface::getGrid()
{
    return m_grid;
}

const sources& ForwardModelInterface::getSrc()
{
    return m_src;
}

const receivers& ForwardModelInterface::getRecv()
{
    return m_recv;
}

const frequenciesGroup& ForwardModelInterface::getFreq()
{
    return m_freq;
}

const forwardModelInput& ForwardModelInterface::getForwardModelInput()
{
    return m_fmInput;
}

std::complex<double>* ForwardModelInterface::calculateResidual(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata)
{
    std::complex<double>* PdataEst = new std::complex<double>[m_freq.nFreq* m_recv.nRecv*m_src.nSrc];

    createPdataEst(PdataEst, chiEst);

    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        m_residual[i] = Pdata[i] - PdataEst[i];
    }

    delete[] PdataEst;

    return m_residual;
}

double ForwardModelInterface::calculateResidualNormSq(std::complex<double> *residual)
{
    double residualSq = normSq(residual, m_freq.nFreq * m_src.nSrc * m_recv.nRecv);

    return residualSq;
}



