#include "forwardModel.h"


forwardModel::forwardModel(const grid2D &grid, const sources &src, const receivers &recv,
                           const frequenciesGroup &freq, const forwardModelInput &fmInput)
    : ForwardModelInterface (grid, src, recv, freq, fmInput),
      greens(), p0(), pTot()
{   
    residual = new std::complex<double>[freq.nFreq * src.nSrc * recv.nRecv];
    kZeta = new std::complex<double>[freq.nFreq * src.nSrc * recv.nRecv];

    std::cout << "Creating Greens function field..." << std::endl;
    this->createGreens();
    std::cout << "Creating P0..." << std::endl;
    this->createP0();

    // initialize kappa
    Kappa = new pressureFieldComplexSerial*[freq.nFreq * src.nSrc * recv.nRecv];
    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        Kappa[i] = new pressureFieldComplexSerial(grid);
    }

    // initialize p_tot1D
    pTot1D = new pressureFieldComplexSerial*[freq.nFreq * src.nSrc];

    int li;

    for (int i=0; i < freq.nFreq ; i++)
    {
        li = i * src.nSrc;

        for (int j=0; j < src.nSrc; j++)
            pTot1D[li + j] = new pressureFieldComplexSerial(*p0[i][j]); // p_tot1D is initialized to p_0 (section 3.3.3 of the report)
    }
}

forwardModel::~forwardModel()
{
    if (this->greens!=nullptr)
        this->deleteGreens();

    if (this->p0!=nullptr)
        this->deleteP0();

    if (this->pTot!=nullptr)
        this->deleteTotalField();

    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
        delete Kappa[i];
    delete[] Kappa;
    Kappa = nullptr;

    for (int i = 0; i < freq.nFreq * src.nSrc; i++)
        delete pTot1D[i];
    delete[] pTot1D;
    pTot1D = nullptr;
}

void forwardModel::createGreens()
{
    greens = new Greens_rect_2D_cpu*[freq.nFreq];

    for (int i=0; i<freq.nFreq; i++)
    {
        greens[i] = new Greens_rect_2D_cpu(grid, Helmholtz2D, src, recv, freq.k[i]);
    }
}

void forwardModel::deleteGreens()
{
    for (int i=0; i < freq.nFreq; i++)
        delete greens[i];
    delete[] greens;
    greens = nullptr;
}

void forwardModel::createP0()
{
    assert(greens != nullptr);
    assert(p0 == nullptr);

    p0 = new pressureFieldComplexSerial**[freq.nFreq];

    for (int i=0; i < freq.nFreq; i++)
    {
        p0[i] = new pressureFieldComplexSerial*[src.nSrc];

        for (int j=0; j < src.nSrc; j++)
        {
            p0[i][j] = new pressureFieldComplexSerial(grid);
            *p0[i][j] = *( greens[i]->GetReceiverCont(j) ) / (freq.k[i] * freq.k[i] * grid.GetCellVolume());
        }
    }
}

void forwardModel::deleteP0()
{
    for (int i=0; i < freq.nFreq; i++)
    {
        for (int j=0; j < src.nSrc; j++)
            delete p0[i][j];

        delete[] p0[i];
    }
    delete[] p0;
    p0 = nullptr;
}

void forwardModel::deleteTotalField()
{
    for (int i=0; i < freq.nFreq; i++)
    {
        for (int j=0; j < src.nSrc; j++)
            delete pTot[i][j];

        delete[] pTot[i];
    }
    delete[] pTot;
    pTot = nullptr;
}

void forwardModel::calculateData(std::complex<double> *pData, pressureFieldSerial chi, Iter2 conjGrad )
{
    this->createTotalField(conjGrad, chi);
    int li, lj;
    for (int i=0; i < freq.nFreq; i++)
    {
        li = i * src.nSrc * recv.nRecv;

        for (int j=0; j < recv.nRecv; j++)
        {
            lj = j * src.nSrc;

            for (int k=0; k < src.nSrc; k++)
            {
                pData[li + lj + k] = Summation( *( greens[i]->GetReceiverCont(j) ) , *pTot[i][k]*chi );
            }
        }
    }
}

void forwardModel::createTotalField(Iter2 conjGrad, pressureFieldSerial chi)
{
    assert(greens != nullptr);
    assert(p0 != nullptr);
    assert(pTot == nullptr);

    this->pTot = new pressureFieldComplexSerial**[freq.nFreq];
        for (int i=0; i < freq.nFreq; i++)
    {
        this->pTot[i] = new pressureFieldComplexSerial*[src.nSrc];

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << freq.nFreq << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j=0; j < src.nSrc; j++)
        {
            pTot[i][j] = new pressureFieldComplexSerial(grid);
            *pTot[i][j] = calcField(*greens[i], chi, *p0[i][j], conjGrad);
        }

        std::cout << "  " << std::endl;
        std::cout << "  " << std::endl;
    }

}

void forwardModel::createTotalField1D(pressureFieldSerial chiEst)
{
    int l_i;
    for (int i=0; i < freq.nFreq; i++)
    {
        l_i = i * src.nSrc;

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << freq.nFreq << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j=0; j < src.nSrc; j++)
            *pTot1D[l_i + j] = calcField(*greens[i], chiEst, *p0[i][j], fmInput.iter2);
    }
}


void forwardModel::calculateKappa()
{
    int li, lj;

    for (int i = 0; i < freq.nFreq; i++)
    {
        li = i * recv.nRecv * src.nSrc;
        for (int j = 0; j < recv.nRecv; j++)
        {
            lj = j * src.nSrc;
            for(int k = 0; k < src.nSrc; k++)
            {
                *Kappa[li + lj + k] = ( *greens[i]->GetReceiverCont(j) ) * (*pTot1D[i*src.nSrc + k]);
            }
        }
    }
}

void forwardModel::intermediateForwardModelStep1()
{
    this->calculateKappa();
}


void forwardModel::calculateResidual(pressureFieldSerial chiEst, const std::complex<double> *const pData)
{
    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        residual[i] = pData[i] - Summation(*Kappa[i], chiEst);
    }
}

std::complex<double>* forwardModel::getResidual()
{
    return residual;
}

double forwardModel::calculateResidualNormSq(double eta)
{
    return eta*normSq(residual, freq.nFreq * src.nSrc * recv.nRecv);
}

void forwardModel::calculateKZeta(pressureFieldSerial zeta)
{
    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        kZeta[i] = Summation( *Kappa[i], zeta);
    }
}

std::complex<double>* forwardModel::intermediateForwardModelStep2(pressureFieldSerial zeta)
{
    calculateKZeta(zeta);
    return kZeta;
}

void forwardModel::calculateKRes(pressureFieldComplexSerial &kRes)
{
    int l_i, l_j;
    kRes.Zero();
    pressureFieldComplexSerial kDummy(this->getGrid());

    for (int i = 0; i < this->freq.nFreq; i++)
    {
        l_i = i*this->recv.nRecv * this->src.nSrc;

        for (int j = 0; j < this->recv.nRecv; j++)
        {
            l_j = j*this->src.nSrc;

            for(int k = 0; k < this->src.nSrc; k++)
            {
                kDummy = *Kappa[l_i + l_j + k];
                kDummy.Conjugate();
                kRes += kDummy * this->getResidual()[l_i + l_j + k];
            }
        }
    }
}

