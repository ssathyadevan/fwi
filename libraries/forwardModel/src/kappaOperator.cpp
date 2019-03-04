#include "kappaOperator.h"

kappaOperator::kappaOperator(ForwardModelInterface *forwardModel)
    :m_grid(), m_freq(), m_src(), m_recv(), m_forwardModel(*forwardModel), m_greens(), m_Kappa(), m_pData()
{
    m_forwardModel = forwardModel;

    m_grid = forwardModel.getGrid();
    m_freq = forwardModel.getFreq();
    m_src  = forwardModel.getSrc();
    m_recv = forwardModel.getRecv();

    this->createGreensSerial();

}

kappaOperator::~kappaOperator()
{
    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        delete m_Kappa[i];
    }

    delete[] m_Kappa;
    m_Kappa = nullptr;

    if (m_greens!=nullptr)
    {
        this->deleteGreensSerial();
    }
}

void kappaOperator::calculateKappa(const pressureFieldSerial &ChiEst)
{
    pressureFieldComplexSerial **pTot1D = forwardModel.getPtot1D(chiEst);

    int li, lj;

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        li = i * m_recv.nRecv * m_src.nSrc;
        for (int j = 0; j < m_recv.nRecv; j++)
        {
            lj = j * m_src.nSrc;
            for(int k = 0; k < m_src.nSrc; k++)
            {
                *m_Kappa[li + lj + k] = ( *m_greens[i]->GetReceiverCont(j) ) * (*pTot1D[i*m_src.nSrc + k]);
            }
        }
    }
}

std::complex<double>* kappaOperator::getResidual()
{

}

double kappaOperator::getResidualNormSq(double eta)
{

}

void kappaOperator::calculateResidual(std::complex<double> *p_data, const pressureFieldSerial &chiEst)
{
    getPdata(*p_data, chiEst);

    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        residual[i] = m_pData[i] - Summation(*m_Kappa[i], chiEst);
    }
}

void kappaOperator::calculateResidualNormSq(double eta)
{
    eta*normSq(residual, freq.nFreq * src.nSrc * recv.nRecv);
}

const pressureFieldSerial& kappaOperator::getPdata(std::complex<double> *p_data, pressureFieldSerial chiEst)
{
    this->createPdata(p_data, chiEst);
    return m_pData;
}

void forwardModel::createPdata(std::complex<double> *pData, const pressureFieldSerial& chiEst)
{
    pressureFieldComplexSerial **pTot = forwardModel.getPtot(chiEst);

    int li, lj;
    for (int i=0; i < m_freq.nFreq; i++)
    {
        li = i * m_src.nSrc * m_recv.nRecv;

        for (int j=0; j < m_recv.nRecv; j++)
        {
            lj = j * m_src.nSrc;

            for (int k=0; k < m_src.nSrc; k++)
            {
                m_pData[li + lj + k] = Summation( *( m_greens[i]->GetReceiverCont(j) ) , *pTot[i][k]*chiEst );
            }
        }
    }
}

void kappaOperator::createGreensSerial()
{
    m_greens = new Greens_rect_2D_cpu*[freq.nFreq];

    for (int i=0; i<freq.nFreq; i++)
    {
        m_greens[i] = new Greens_rect_2D_cpu(grid, Helmholtz2D, src, recv, freq.k[i]);
    }
}

void kappaOperator::deleteGreensSerial()
{
    for (int i=0; i < freq.nFreq; i++)
    {
        delete m_greens[i];
    }

    delete[] m_greens;
    m_greens = nullptr;
}
