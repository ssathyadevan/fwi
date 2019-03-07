#include "forwardModelBasicOptimization.h"

forwardModelBasicOptimization::forwardModelBasicOptimization(const grid2D &grid, const sources &src, const receivers &recv,
                           const frequenciesGroup &freq, const forwardModelInput &fmInput)
    : ForwardModelInterface (grid, src, recv, freq, fmInput),
      m_greens(), m_P0(), m_Ptot(), m_Ptot1D(), m_Kappa()
{
    this->createGreensSerial();
    this->createP0();

    // Initialize Ptot and Ptot1D

    m_Ptot = new pressureFieldComplexSerial**[m_freq.nFreq];

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        m_Ptot[i] = new pressureFieldComplexSerial*[m_src.nSrc];

        for (int j = 0; j < m_src.nSrc; j++)
        {
            m_Ptot[i][j] = new pressureFieldComplexSerial(m_grid);
        }
    }

    m_Ptot1D = new pressureFieldComplexSerial*[freq.nFreq * src.nSrc];

    int li;

    for (int i = 0; i < freq.nFreq ; i++)
    {
        li = i * src.nSrc;

        for (int j = 0; j < src.nSrc; j++)
        {
            m_Ptot1D[li + j] = new pressureFieldComplexSerial(*m_P0[i][j]);
        }
    }

    // initialize kappa
    m_Kappa = new pressureFieldComplexSerial*[freq.nFreq * src.nSrc * recv.nRecv];

    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        m_Kappa[i] = new pressureFieldComplexSerial(m_grid);
    }

}

forwardModelBasicOptimization::~forwardModelBasicOptimization()
{
    if (m_greens != nullptr)
    {
        this->deleteGreensSerial();
    }

    if (m_P0 != nullptr)
    {
        this->deleteP0();
    }

    if (m_Ptot != nullptr)
    {
        this->deletePtot();
    }

    if (m_Ptot1D != nullptr)
    {
        this->deletePtot1D();
    }

    if (m_Kappa != nullptr)
    {
        this->deleteKappa();
    }

}

void forwardModelBasicOptimization::createP0()
{
    assert(m_greens != nullptr);
    assert(m_P0 == nullptr);

    m_P0 = new pressureFieldComplexSerial**[m_freq.nFreq];

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        m_P0[i] = new pressureFieldComplexSerial*[m_src.nSrc];

        for (int j = 0; j < m_src.nSrc; j++)
        {
            m_P0[i][j] = new pressureFieldComplexSerial(m_grid);
            *m_P0[i][j] = *( m_greens[i]->GetReceiverCont(j) ) / (m_freq.k[i] * m_freq.k[i] * m_grid.GetCellVolume());
        }
    }
}

void forwardModelBasicOptimization::deleteP0()
{
    for (int i = 0; i < m_freq.nFreq; i++)
    {
        for (int j = 0; j < m_src.nSrc; j++)
        {
            delete m_P0[i][j];
        }

        delete[] m_P0[i];
    }

    delete[] m_P0;
    m_P0 = nullptr;
}

void forwardModelBasicOptimization::createGreensSerial()
{
    m_greens = new Greens_rect_2D_cpu*[m_freq.nFreq];

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        m_greens[i] = new Greens_rect_2D_cpu(m_grid, Helmholtz2D, m_src, m_recv, m_freq.k[i]);
    }
}

void forwardModelBasicOptimization::deleteGreensSerial()
{
    for (int i=0; i < m_freq.nFreq; i++)
    {
        delete m_greens[i];
    }

    delete[] m_greens;
    m_greens = nullptr;
}

void forwardModelBasicOptimization::createPtot(const pressureFieldSerial &chiEst)
{
    assert(m_greens != nullptr);
    assert(m_P0 != nullptr);
    assert(m_Ptot1D != nullptr);

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        for (int j = 0; j < m_src.nSrc; j++)
        {
            *m_Ptot[i][j] = calcTotalField(*m_greens[i], chiEst, *m_P0[i][j]);
        }
    }
}

void forwardModelBasicOptimization::createPtot1D(const pressureFieldSerial &chiEst)
{
    assert(m_greens != nullptr);
    assert(m_P0 != nullptr);
    assert(m_Ptot1D != nullptr);

    int li;

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        li = i * m_src.nSrc;

        for (int j = 0; j < m_src.nSrc; j++)
        {
            *m_Ptot1D[li + j] = calcTotalField(*m_greens[i], chiEst, *m_P0[i][j]);
        }
    }
}


void forwardModelBasicOptimization::deletePtot()
{
    for (int i = 0; i < m_freq.nFreq; i++)
    {
        for (int j = 0; j < m_src.nSrc; j++)
        {
            delete m_Ptot[i][j];
        }

        delete[] m_Ptot[i];
    }

    delete[] m_Ptot;
    m_Ptot = nullptr;
}

void forwardModelBasicOptimization::deletePtot1D()
{
    for (int i = 0; i < m_freq.nFreq * m_src.nSrc; i++)
    {
        delete m_Ptot1D[i];
    }

    delete[] m_Ptot1D;
    m_Ptot1D = nullptr;
}

pressureFieldComplexSerial forwardModelBasicOptimization::calcTotalField(const Greens_rect_2D_cpu &G, const pressureFieldSerial &chi, const pressureFieldComplexSerial &p_init)
{
    assert(&G.GetGrid() == &p_init.GetGrid());

    Iter2 iter2 = m_fmInput.iter2;

    pressureFieldComplexSerial chi_p(m_grid), chi_p_old(m_grid);
    pressureFieldComplexSerial dW(m_grid), p_tot(m_grid);

    chi_p_old.Zero();

    p_tot = p_init;

    double res = 0.0;

    for(int it = 0; it < iter2.n; it++)
    {
        chi_p = p_tot * chi;

        dW = chi_p - chi_p_old;

        double dWNorm = dW.Norm();
        double chi_pNorm = chi_p.Norm();

        res = dWNorm / chi_pNorm;

        if(res < iter2.tolerance && it != 0)
        {
            if(true)
            {
                std::string itstring = std::to_string(it);
                std::string line_to_print = "Convergence after " + itstring + "iterations";
                std::cout << line_to_print << std::endl;
            }

            break;
        }

        p_tot += G.dot1(dW);

        chi_p_old = chi_p;
        chi_p.Zero();

    }

    return p_tot;
}

void forwardModelBasicOptimization::initializeForwardModel(const pressureFieldSerial &chiEst)
{
    this ->calculateKappa(chiEst);    
}

void forwardModelBasicOptimization::postInitializeForwardModel(const pressureFieldSerial &chiEst)
{
    this ->createPtot1D(chiEst);
}

void forwardModelBasicOptimization::calculateKappa(const pressureFieldSerial &chiEst)
{
    int li, lj;

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        li = i * m_recv.nRecv * m_src.nSrc;

        for (int j = 0; j < m_recv.nRecv; j++)
        {
            lj = j * m_src.nSrc;

            for(int k = 0; k < m_src.nSrc; k++)
            {
                *m_Kappa[li + lj + k] = ( *m_greens[i]->GetReceiverCont(j) ) * (*m_Ptot1D[i * m_src.nSrc + k]);
            }
        }
    }
}

void forwardModelBasicOptimization::deleteKappa()
{
    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        delete m_Kappa[i];
    }

    delete[] m_Kappa;
    m_Kappa = nullptr;
}

void forwardModelBasicOptimization::createPdataEst(std::complex<double> *Pdata, const pressureFieldSerial &chiEst)
{
    this->createPtot(chiEst);

    //std::complex<double>* PdataEst = new std::complex<double>[m_freq.nFreq* m_recv.nRecv*m_src.nSrc];

    int li, lj;

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        li = i * m_src.nSrc * m_recv.nRecv;

        for (int j = 0; j < m_recv.nRecv; j++)
        {
            lj = j * m_src.nSrc;

            for (int k = 0; k < m_src.nSrc; k++)
            {
                Pdata[li + lj + k] = Summation( ( *m_greens[i]->GetReceiverCont(j) ) , *m_Ptot[i][k] * chiEst );
            }
        }
    }

//    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
//    {
//        Pdata[i] = Summation(*m_Kappa[i], chiEst);
//    }

}

std::complex<double>* forwardModelBasicOptimization::createKappaOperator(const pressureFieldSerial &CurrentPressureFieldSerial)
{
    std::complex<double>* kOperator = new std::complex<double>[m_freq.nFreq * m_src.nSrc * m_recv.nRecv];

    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        kOperator[i] = Summation( *m_Kappa[i], CurrentPressureFieldSerial);
    }

    return kOperator;
}

std::complex<double>* forwardModelBasicOptimization::createKappaOperator(const pressureFieldComplexSerial &CurrentPressureFieldComplexSerial)
{
    auto kOperator = new std::complex<double>[m_freq.nFreq * m_src.nSrc * m_recv.nRecv];

    for (int i = 0; i < m_freq.nFreq * m_src.nSrc * m_recv.nRecv; i++)
    {
        kOperator[i] = Summation( *m_Kappa[i], CurrentPressureFieldComplexSerial);
    }

    return kOperator;
}

void forwardModelBasicOptimization::calculateOperatorMapStoD(pressureFieldComplexSerial &kRes, std::complex<double>* res)
{
    int l_i, l_j;

    kRes.Zero();

    pressureFieldComplexSerial kDummy(m_grid);

    for (int i = 0; i < m_freq.nFreq; i++)
    {
        l_i = i * m_recv.nRecv * m_src.nSrc;

        for (int j = 0; j < m_recv.nRecv; j++)
        {
            l_j = j * m_src.nSrc;

            for(int k = 0; k < m_src.nSrc; k++)
            {
                kDummy = *m_Kappa[l_i + l_j + k];
                kDummy.Conjugate();

                kRes += kDummy * res[l_i + l_j + k];
            }
        }
    }
}

