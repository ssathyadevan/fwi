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
    std::complex<double>* pDataEst = createPdataEst(chiEst);

    std::complex<double>* residual = new std::complex<double>[m_freq.nFreq * m_src.nSrc * m_recv.nRecv];

    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        residual[i] = Pdata[i] - pDataEst[i];
    }

    delete[] pDataEst;

    return residual;
}

double ForwardModelInterface::calculateResidualNormSq(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata)
{
    std::complex<double>* residual = calculateResidual(chiEst, Pdata);

    double residualSq = normSq(residual, m_freq.nFreq * m_src.nSrc * m_recv.nRecv);

    delete[] residual;

    return residualSq;
}



