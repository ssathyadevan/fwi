#include "integralForwardModel.h"

IntegralForwardModel::IntegralForwardModel(const grid2D &grid, const sources &src, const receivers &recv,
                           const frequenciesGroup &freq, const integralForwardModelInput &fmInput)
    : ForwardModelInterface (grid, src, recv, freq),
      _Greens(), _p0(), _pTot(), _Kappa(), _fmInput(fmInput)
{
    std::cout << "Creating Greens function field..." << std::endl;
    createGreens();
    std::cout << "Creating p0..." << std::endl;
    createP0();
    createPTot(freq, src);
    createKappa(freq, src, recv);
}

IntegralForwardModel::~IntegralForwardModel()
{
    if (_Greens != nullptr)
        this->deleteGreens();

    if (_p0 != nullptr)
        this->deleteP0();

    if (_pTot != nullptr)
        this->deletePtot();

    if (_Kappa != nullptr)
        this->deleteKappa();
}

void IntegralForwardModel::createP0()
{
    assert(_Greens != nullptr);
    assert(_p0 == nullptr);

    _p0 = new pressureFieldComplexSerial**[_freq.nFreq];

    for (int i = 0; i < _freq.nFreq; i++)
    {
        _p0[i] = new pressureFieldComplexSerial*[_src.nSrc];

        for (int j = 0; j < _src.nSrc; j++)
        {
            _p0[i][j] = new pressureFieldComplexSerial(_grid);
            *_p0[i][j] = *( _Greens[i]->GetReceiverCont(j) ) / (_freq.k[i] * _freq.k[i] * _grid.GetCellVolume());
        }
    }
}

void IntegralForwardModel::deleteP0()
{
    for (int i = 0; i < _freq.nFreq; i++)
    {
        for (int j = 0; j < _src.nSrc; j++)
        {
            delete _p0[i][j];
        }

        delete[] _p0[i];
    }

    delete[] _p0;
    _p0 = nullptr;
}

void IntegralForwardModel::createGreens()
{
    _Greens = new Greens_rect_2D_cpu*[_freq.nFreq];

    for (int i = 0; i < _freq.nFreq; i++)
    {
        _Greens[i] = new Greens_rect_2D_cpu(_grid, GreensFunctions::Helmholtz2D, _src, _recv, _freq.k[i]);
    }
}

void IntegralForwardModel::deleteGreens()
{
    for (int i=0; i < _freq.nFreq; i++)
    {
        delete _Greens[i];
    }

    delete[] _Greens;
    _Greens = nullptr;
}

void IntegralForwardModel::createPTot(const frequenciesGroup &freq, const sources &src)
{
    _pTot = new pressureFieldComplexSerial*[freq.nFreq * src.nSrc];

    int li;

    for (int i = 0; i < freq.nFreq ; i++)
    {
        li = i * src.nSrc;

        for (int j = 0; j < src.nSrc; j++)
        {
            _pTot[li + j] = new pressureFieldComplexSerial(*_p0[i][j]);
        }
    }
}

void IntegralForwardModel::deletePtot()
{
    for (int i = 0; i < _freq.nFreq * _src.nSrc; i++)
    {
        delete _pTot[i];
    }

    delete[] _pTot;
    _pTot = nullptr;
}

void IntegralForwardModel::createKappa(const frequenciesGroup &freq, const sources &src, const receivers &recv)
{
    _Kappa = new pressureFieldComplexSerial*[freq.nFreq * src.nSrc * recv.nRecv];

    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        _Kappa[i] = new pressureFieldComplexSerial(_grid);
    }
}

void IntegralForwardModel::deleteKappa()
{
    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        delete _Kappa[i];
    }

    delete[] _Kappa;
    _Kappa = nullptr;
}

pressureFieldComplexSerial IntegralForwardModel::calcTotalField(const Greens_rect_2D_cpu &G, const pressureFieldSerial &chi, const pressureFieldComplexSerial &p_init)
{
    assert(&G.GetGrid() == &p_init.GetGrid());

    Iter2 iter2 = _fmInput.iter2;

    pressureFieldComplexSerial chi_p(_grid), chi_p_old(_grid);
    pressureFieldComplexSerial dW(_grid), p_tot(_grid), f_rhs(_grid), matA_j(_grid);

    int n_cell = _grid.GetNumberOfGridPoints();

    std::vector< pressureFieldComplexSerial >                       phi;
    Matrix< std::complex<double> , Dynamic, Dynamic, ColMajor >     matA;
    Matrix< std::complex<double> , Dynamic, 1, ColMajor >           b_f_rhs;
    Matrix< std::complex<double> , Dynamic, 1, ColMajor >           alpha;

    std::complex<double>  *rhs_data, *matA_j_data;

    matA.conservativeResize(n_cell, 1);
    b_f_rhs.conservativeResize(n_cell, 1);

    double res = 0.0;

    chi_p_old.Zero();

    p_tot = p_init;

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

        if (iter2.calcAlpha)
        {
            phi.push_back(G.ContractWithField(dW));
            f_rhs = G.ContractWithField(chi*p_init);

            rhs_data = f_rhs.GetDataPtr();

            matA_j = phi[it] - G.ContractWithField(chi*phi[it]);
            matA_j_data = matA_j.GetDataPtr();

            matA.conservativeResize(NoChange,it+1);
            alpha.conservativeResize(it+1);

            for(int i=0; i<n_cell; i++)
            {
                matA(i,it) = matA_j_data[i];
                b_f_rhs(i) = rhs_data[i];
            }

            alpha = matA.jacobiSvd(ComputeThinU | ComputeThinV).solve(b_f_rhs);

            p_tot = p_init;

            for(int j=0; j<it+1; j++)
            {
                p_tot += alpha[j]*phi[j];
            }

        } else {

            p_tot += G.dot1(dW);
        }

        chi_p_old = chi_p;
        chi_p.Zero();

    }

    if(res >= iter2.tolerance)
    {
        std::cout << "No convergence after " <<  iter2.n << " iterations." << "Res = " << res << std::endl;
    }

    return p_tot;
}

void IntegralForwardModel::calculatePTot(const pressureFieldSerial &chiEst)
{
    assert(_Greens  != nullptr);
    assert(_p0      != nullptr);

    int li;

    for (int i = 0; i < _freq.nFreq; i++)
    {
        li = i * _src.nSrc;

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << _freq.nFreq << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j = 0; j < _src.nSrc; j++)
        {
            *_pTot[li + j] = calcTotalField(*_Greens[i], chiEst, *_p0[i][j]);
        }
    }
}

void IntegralForwardModel::calculatePData(const pressureFieldSerial &chiEst, std::complex<double> *kOperator)
{
    applyKappa(chiEst, kOperator);
}

void IntegralForwardModel::calculateKappa()
{
    int li, lj;

    for (int i = 0; i < _freq.nFreq; i++)
    {
        li = i * _recv.nRecv * _src.nSrc;

        for (int j = 0; j < _recv.nRecv; j++)
        {
            lj = j * _src.nSrc;

            for(int k = 0; k < _src.nSrc; k++)
            {
                *_Kappa[li + lj + k] = ( *_Greens[i]->GetReceiverCont(j) ) * (*_pTot[i * _src.nSrc + k]);
            }
        }
    }
}

void IntegralForwardModel::mapDomainToSignal(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double> *kOperator)
{
    applyKappa(CurrentPressureFieldSerial, kOperator);
}

void IntegralForwardModel::applyKappa(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double> *kOperator)
{
    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {       
        kOperator[i] = Summation( *_Kappa[i], CurrentPressureFieldSerial);
    }
}

//void IntegralForwardModel::createKappaOperator(const pressureFieldComplexSerial &CurrentPressureFieldComplexSerial, std::complex<double>* kOperator)
//{
//    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
//    {
//        kOperator[i] = Summation( *_Kappa[i], CurrentPressureFieldComplexSerial);
//    }
//}

void IntegralForwardModel::getUpdateDirectionInformation(std::complex<double>* res, pressureFieldComplexSerial &kRes)
{
    int l_i, l_j;

    kRes.Zero();

    pressureFieldComplexSerial kDummy(_grid);

    for (int i = 0; i < _freq.nFreq; i++)
    {
        l_i = i * _recv.nRecv * _src.nSrc;

        for (int j = 0; j < _recv.nRecv; j++)
        {
            l_j = j * _src.nSrc;

            for(int k = 0; k < _src.nSrc; k++)
            {
                kDummy = *_Kappa[l_i + l_j + k];
                kDummy.Conjugate();

                kRes += kDummy * res[l_i + l_j + k];
            }
        }
    }
}

