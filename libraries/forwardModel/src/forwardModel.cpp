#include "forwardModel.h"


forwardModel::forwardModel(const grid2D &grid, const sources &src, const receivers &recv,
                           const frequenciesGroup &freq, genericInput gInput, forwardModelInput fmInput)
    : ForwardModelInterface (grid, src, recv, freq, gInput, fmInput),
      greens(), p0(), pTot()
{
    residual = new std::complex<double>[gInput.freq.nTotal*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src];
    kZeta = new std::complex<double>[gInput.freq.nTotal*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src];

    std::cout << "Creating Greens function field..." << std::endl;
    this->createGreens();
    std::cout << "Creating P0..." << std::endl;
    this->createP0();

    // initialize kappa
    Kappa = new pressureFieldComplexSerial*[gInput.freq.nTotal*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src];
    for (int i = 0; i < gInput.freq.nTotal*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src; i++)
    {
        Kappa[i] = new pressureFieldComplexSerial(grid);
    }

    // initialize p_tot1D
    pTot1D = new pressureFieldComplexSerial*[gInput.freq.nTotal*gInput.nSourcesReceivers.src];
    int li;
    for (int i=0; i<gInput.freq.nTotal; i++)
    {
        li = i*gInput.nSourcesReceivers.src;
        for (int j=0; j<gInput.nSourcesReceivers.src;j++)
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

    for (int i = 0; i < gInput.freq.nTotal*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src; i++)
        delete Kappa[i];
    delete[] Kappa;
    Kappa = nullptr;

    for (int i = 0; i < gInput.freq.nTotal*gInput.nSourcesReceivers.src; i++)
        delete pTot1D[i];
    delete[] pTot1D;
    pTot1D = nullptr;
}

void forwardModel::createGreens()
{
    greens = new Greens_rect_2D_cpu*[gInput.freq.nTotal];

    for (int i=0; i<gInput.freq.nTotal; i++)
    {
        greens[i] = new Greens_rect_2D_cpu(grid, Helmholtz2D, src, recv, freq.k[i]);
    }
}

void forwardModel::deleteGreens()
{
    for (int i=0; i<gInput.freq.nTotal; i++)
        delete greens[i];
    delete[] greens;
    greens = nullptr;
}

void forwardModel::createP0()
{
    assert(greens != nullptr);
    assert(p0 == nullptr);

    p0 = new pressureFieldComplexSerial**[gInput.freq.nTotal];

    for (int i=0; i<gInput.freq.nTotal; i++)
    {
        p0[i] = new pressureFieldComplexSerial*[gInput.nSourcesReceivers.src];

        for (int j=0; j<gInput.nSourcesReceivers.src; j++)
        {
            p0[i][j] = new pressureFieldComplexSerial(grid);
            *p0[i][j] = *( greens[i]->GetReceiverCont(j) ) / (freq.k[i] * freq.k[i] * grid.GetCellVolume());
        }
    }
}

void forwardModel::deleteP0()
{
    for (int i=0; i<gInput.freq.nTotal; i++)
    {
        for (int j=0; j<gInput.nSourcesReceivers.src; j++)
            delete p0[i][j];

        delete[] p0[i];
    }
    delete[] p0;
    p0 = nullptr;
}

void forwardModel::deleteTotalField()
{
    for (int i=0; i<gInput.freq.nTotal; i++)
    {
        for (int j=0; j<gInput.nSourcesReceivers.src; j++)
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
    for (int i=0; i<gInput.freq.nTotal; i++)
    {
        li = i*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src;
        for (int j=0; j<gInput.nSourcesReceivers.rec; j++)
        {
            lj = j*gInput.nSourcesReceivers.src;
            for (int k=0; k<gInput.nSourcesReceivers.src; k++)
            {
                pData[li + lj + k] = Summation( *( greens[i]->GetReceiverCont(j) ) , *pTot[i][k]*chi );
            }
        }
    }
}

void forwardModel::createTotalField(Iter2 conjGrad, pressureFieldSerial chi)
{
    assert(this->greens != nullptr);
    assert(this->p0 != nullptr);
    assert(this->pTot == nullptr);

    this->pTot = new pressureFieldComplexSerial**[gInput.freq.nTotal];
        for (int i=0; i<this->gInput.freq.nTotal; i++)
    {
        this->pTot[i] = new pressureFieldComplexSerial*[gInput.nSourcesReceivers.src];

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << gInput.freq.nTotal << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j=0; j<this->gInput.nSourcesReceivers.src; j++)
        {
            this->pTot[i][j] = new pressureFieldComplexSerial(this->grid);
            *this->pTot[i][j] = calcField(*this->greens[i], chi, *this->p0[i][j], conjGrad);
        }

        std::cout << "  " << std::endl;
        std::cout << "  " << std::endl;
    }

}

void forwardModel::createTotalField1D(Iter2 conjGrad, pressureFieldSerial chiEst)
{
    int l_i;
    for (int i=0; i<gInput.freq.nTotal; i++)
    {
        l_i = i*gInput.nSourcesReceivers.src;

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << gInput.freq.nTotal << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j=0; j<gInput.nSourcesReceivers.src;j++)
            *pTot1D[l_i + j] = calcField(*greens[i], chiEst, *p0[i][j], conjGrad);
    }
}

forwardModelInput forwardModel::getForwardModelInput()
{
    return fmInput;
}
genericInput forwardModel::getGenericInput()
{
    return gInput;
}


void forwardModel::calculateKappa()
{
    int li, lj;

    for (int i = 0; i < gInput.freq.nTotal; i++)
    {
        li = i*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src;
        for (int j = 0; j < gInput.nSourcesReceivers.rec; j++)
        {
            lj = j*gInput.nSourcesReceivers.src;
            for(int k = 0; k < gInput.nSourcesReceivers.src; k++)
            {
                *Kappa[li + lj + k] = ( *greens[i]->GetReceiverCont(j) ) * (*pTot1D[i*gInput.nSourcesReceivers.src + k]);
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
    for (int i = 0; i < gInput.freq.nTotal*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src; i++)
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
    return eta*normSq(residual,gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src*gInput.freq.nTotal);
}

void forwardModel::calculateKZeta(pressureFieldSerial zeta)
{
    for (int i = 0; i < gInput.freq.nTotal*gInput.nSourcesReceivers.rec*gInput.nSourcesReceivers.src; i++)
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

    for (int i = 0; i < this->getGenericInput().freq.nTotal; i++)
    {
        l_i = i*this->getGenericInput().nSourcesReceivers.rec * this->getGenericInput().nSourcesReceivers.src;

        for (int j = 0; j < this->getGenericInput().nSourcesReceivers.rec; j++)
        {
            l_j = j*this->getGenericInput().nSourcesReceivers.src;

            for(int k = 0; k < this->getGenericInput().nSourcesReceivers.src; k++)
            {
                kDummy = *Kappa[l_i + l_j + k];
                kDummy.Conjugate();
                kRes += kDummy * this->getResidual()[l_i + l_j + k];
            }
        }
    }
}

