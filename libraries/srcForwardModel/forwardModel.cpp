#include "forwardModel.h"


ForwardModel::ForwardModel(const grid_rect_2D &grid, const Sources_rect_2D &src, const Receivers_rect_2D &recv,
                           const Frequencies_group &freq, ProfileInterface &profiler, Input input)
    : ForwardModelInterface (grid, src, recv, freq, profiler, input),
      greens(), p0(), pTot(), profiler(profiler)
{
    residual = new std::complex<double>[input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src];
    kZeta = new std::complex<double>[input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src];

    std::cout << "Creating Greens function field..." << std::endl;
    this->createGreens();
    std::cout << "Creating P0..." << std::endl;
    this->createP0();

    // initialize kappa
    Kappa = new volComplexField_rect_2D_cpu*[input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src];
    for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
    {
        Kappa[i] = new volComplexField_rect_2D_cpu(grid);
    }

    // initialize p_tot1D
    pTot1D = new volComplexField_rect_2D_cpu*[input.freq.nTotal*input.nSourcesReceivers.src];
    int li;
    for (int i=0; i<input.freq.nTotal; i++)
    {
        li = i*input.nSourcesReceivers.src;
        for (int j=0; j<input.nSourcesReceivers.src;j++)
            pTot1D[li + j] = new volComplexField_rect_2D_cpu(*p0[i][j]); // p_tot1D is initialized to p_0 (section 3.3.3 of the report)
    }
}

ForwardModel::~ForwardModel()
{
    if (this->greens!=nullptr)
        this->deleteGreens();

    if (this->p0!=nullptr)
        this->deleteP0();

    if (this->pTot!=nullptr)
        this->deleteTotalField();

    for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
        delete Kappa[i];
    delete[] Kappa;
    Kappa = nullptr;

    for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.src; i++)
        delete pTot1D[i];
    delete[] pTot1D;
    pTot1D = nullptr;
}

void ForwardModel::createGreens()
{
    greens = new Greens_rect_2D_cpu*[input.freq.nTotal];

    for (int i=0; i<input.freq.nTotal; i++)
    {
        greens[i] = new Greens_rect_2D_cpu(grid, Helmholtz2D, src, recv, freq.k[i]);
    }
}

void ForwardModel::deleteGreens()
{
    for (int i=0; i<input.freq.nTotal; i++)
        delete greens[i];
    delete[] greens;
    greens = nullptr;
}

void ForwardModel::createP0()
{
    assert(greens != nullptr);
    assert(p0 == nullptr);

    p0 = new volComplexField_rect_2D_cpu**[input.freq.nTotal];

    for (int i=0; i<input.freq.nTotal; i++)
    {
        p0[i] = new volComplexField_rect_2D_cpu*[input.nSourcesReceivers.src];

        for (int j=0; j<input.nSourcesReceivers.src; j++)
        {
            p0[i][j] = new volComplexField_rect_2D_cpu(grid);
            *p0[i][j] = *( greens[i]->GetReceiverCont(j) ) / (freq.k[i] * freq.k[i] * grid.GetCellVolume());
        }
    }
}

void ForwardModel::deleteP0()
{
    for (int i=0; i<input.freq.nTotal; i++)
    {
        for (int j=0; j<input.nSourcesReceivers.src; j++)
            delete p0[i][j];

        delete[] p0[i];
    }
    delete[] p0;
    p0 = nullptr;
}

void ForwardModel::deleteTotalField()
{
    for (int i=0; i<input.freq.nTotal; i++)
    {
        for (int j=0; j<input.nSourcesReceivers.src; j++)
            delete pTot[i][j];

        delete[] pTot[i];
    }
    delete[] pTot;
    pTot = nullptr;
}

void ForwardModel::calculateData(std::complex<double> *pData, volField_rect_2D_cpu chi, Iter2 conjGrad )
{
    this->createTotalField(conjGrad, chi);
    int li, lj;
    for (int i=0; i<input.freq.nTotal; i++)
    {
        li = i*input.nSourcesReceivers.rec*input.nSourcesReceivers.src;
        for (int j=0; j<input.nSourcesReceivers.rec; j++)
        {
            lj = j*input.nSourcesReceivers.src;
            for (int k=0; k<input.nSourcesReceivers.src; k++)
            {
                pData[li + lj + k] = Summation( *( greens[i]->GetReceiverCont(j) ) , *pTot[i][k]*chi );
            }
        }
    }
}

void ForwardModel::createTotalField(Iter2 conjGrad, volField_rect_2D_cpu chi)
{
    assert(this->greens != nullptr);
    assert(this->p0 != nullptr);
    assert(this->pTot == nullptr);

    std::string name = "createTotalFieldCurrentProcessor";

    this->pTot = new volComplexField_rect_2D_cpu**[input.freq.nTotal];
    this->profiler.StartRegion(name);
    for (int i=0; i<this->input.freq.nTotal; i++)
    {
        this->pTot[i] = new volComplexField_rect_2D_cpu*[input.nSourcesReceivers.src];

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << input.freq.nTotal << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j=0; j<this->input.nSourcesReceivers.src; j++)
        {
            this->pTot[i][j] = new volComplexField_rect_2D_cpu(this->grid);
            *this->pTot[i][j] = calcField(*this->greens[i], chi, *this->p0[i][j], conjGrad);
        }

        std::cout << "  " << std::endl;
        std::cout << "  " << std::endl;
    }
    this->profiler.EndRegion();
}

void ForwardModel::createTotalField1D(Iter2 conjGrad, volField_rect_2D_cpu chiEst)
{
    int l_i;
    for (int i=0; i<input.freq.nTotal; i++)
    {
        l_i = i*input.nSourcesReceivers.src;

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << input.freq.nTotal << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j=0; j<input.nSourcesReceivers.src;j++)
            *pTot1D[l_i + j] = calcField(*greens[i], chiEst, *p0[i][j], conjGrad);
    }
}

ProfileInterface& ForwardModel::getProfiler()
{
    return profiler;
}

Input ForwardModel::getInput()
{
    return input;
}


void ForwardModel::calculateKappa()
{
    int li, lj;

    for (int i = 0; i < input.freq.nTotal; i++)
    {
        li = i*input.nSourcesReceivers.rec*input.nSourcesReceivers.src;
        for (int j = 0; j < input.nSourcesReceivers.rec; j++)
        {
            lj = j*input.nSourcesReceivers.src;
            for(int k = 0; k < input.nSourcesReceivers.src; k++)
            {
                *Kappa[li + lj + k] = ( *greens[i]->GetReceiverCont(j) ) * (*pTot1D[i*input.nSourcesReceivers.src + k]);
            }
        }
    }
}

void ForwardModel::intermediateForwardModelStep1()
{
    this->calculateKappa();
}


void ForwardModel::calculateResidual(volField_rect_2D_cpu chiEst, const std::complex<double> *const pData)
{
    for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
    {
        residual[i] = pData[i] - Summation(*Kappa[i], chiEst);
    }
}

std::complex<double>* ForwardModel::getResidual()
{
    return residual;
}

double ForwardModel::calculateResidualNormSq(double eta)
{
    return eta*normSq(residual,input.nSourcesReceivers.rec*input.nSourcesReceivers.src*input.freq.nTotal);
}

void ForwardModel::calculateKZeta(volField_rect_2D_cpu zeta)
{
    for (int i = 0; i < input.freq.nTotal*input.nSourcesReceivers.rec*input.nSourcesReceivers.src; i++)
    {
        kZeta[i] = Summation( *Kappa[i], zeta);
    }
}

std::complex<double>* ForwardModel::intermediateForwardModelStep2(volField_rect_2D_cpu zeta)
{
    calculateKZeta(zeta);
    return kZeta;
}

void ForwardModel::calculateKRes(volComplexField_rect_2D_cpu &kRes)
{
    int l_i, l_j;
    kRes.Zero();
    volComplexField_rect_2D_cpu kDummy(this->getGrid());

    for (int i = 0; i < this->getInput().freq.nTotal; i++)
    {
        l_i = i*this->getInput().nSourcesReceivers.rec * this->getInput().nSourcesReceivers.src;

        for (int j = 0; j < this->getInput().nSourcesReceivers.rec; j++)
        {
            l_j = j*this->getInput().nSourcesReceivers.src;

            for(int k = 0; k < this->getInput().nSourcesReceivers.src; k++)
            {
                kDummy = *Kappa[l_i + l_j + k];
                kDummy.Conjugate();
                kRes += kDummy * this->getResidual()[l_i + l_j + k];
            }
        }
    }
}

