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


std::complex<double>* ForwardModelInterface::getResidual(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata)
{
    this->calculateResidual(chiEst, *Pdata);
    return m_residual;
}

double ForwardModelInterface::getResidualNormSq(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata)
{
    this->calculateResidualNormSq(chiEst, *Pdata);
    return m_residualSq;
}

void ForwardModelInterface::calculateResidual(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata)
{
    pressureFieldSerial** pDataEst = getPdataEst(chiEst);

    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        m_residual[i] = Pdata[i] - pDataEst;
    }
}

void ForwardModelInterface::calculateResidualNormSq(const pressureFieldSerial &chiEst, const std::complex<double> *Pdata)
{
    this->calculateResidual(chiEst, *Pdata);

    m_residualSq = normSq(m_residual, m_freq.nFreq * m_src.nSrc * m_recv.nRecv);
}



