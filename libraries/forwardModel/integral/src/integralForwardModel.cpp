#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"

IntegralForwardModel::IntegralForwardModel(
    const grid2D &grid, const sources &src, const receivers &recv, const frequenciesGroup &freq, const integralForwardModelInput &fmInput) :
    forwardModelInterface(grid, src, recv, freq),
    _Greens(), _p0(), _pTot(), _Kappa(), _fmInput(fmInput)
{
    L_(linfo) << "Creating Greens function field...";
    createGreens();
    L_(linfo) << "Creating p0...";
    createP0();
    createPTot(freq, src);
    createKappa(freq, src, recv);
}

IntegralForwardModel::~IntegralForwardModel()
{
    if(_Greens != nullptr)
        this->deleteGreens();

    if(_p0 != nullptr)
        this->deleteP0();

    if(_pTot != nullptr)
        this->deletePtot();

    if(_Kappa != nullptr)
        this->deleteKappa();
}

void IntegralForwardModel::createP0()
{
    assert(_Greens != nullptr);
    assert(_p0 == nullptr);

    _p0 = new complexDataGrid2D **[_freq.nFreq];

    for(int i = 0; i < _freq.nFreq; i++)
    {
        _p0[i] = new complexDataGrid2D *[_src.nSrc];

        for(int j = 0; j < _src.nSrc; j++)
        {
            _p0[i][j] = new complexDataGrid2D(_grid);
            *_p0[i][j] = *(_Greens[i]->getReceiverCont(j)) / (_freq.k[i] * _freq.k[i] * _grid.getCellVolume());
        }
    }
}

void IntegralForwardModel::deleteP0()
{
    for(int i = 0; i < _freq.nFreq; i++)
    {
        for(int j = 0; j < _src.nSrc; j++)
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
    _Greens = new greensRect2DCpu *[_freq.nFreq];

    for(int i = 0; i < _freq.nFreq; i++)
    {
        _Greens[i] = new greensRect2DCpu(_grid, greensFunctions::Helmholtz2D, _src, _recv, _freq.k[i]);
    }
}

void IntegralForwardModel::deleteGreens()
{
    for(int i = 0; i < _freq.nFreq; i++)
    {
        delete _Greens[i];
    }

    delete[] _Greens;
    _Greens = nullptr;
}

void IntegralForwardModel::createPTot(const frequenciesGroup &freq, const sources &src)
{
    _pTot = new complexDataGrid2D *[freq.nFreq * src.nSrc];

    int li;

    for(int i = 0; i < freq.nFreq; i++)
    {
        li = i * src.nSrc;

        for(int j = 0; j < src.nSrc; j++)
        {
            _pTot[li + j] = new complexDataGrid2D(*_p0[i][j]);
        }
    }
}

void IntegralForwardModel::deletePtot()
{
    for(int i = 0; i < _freq.nFreq * _src.nSrc; i++)
    {
        delete _pTot[i];
    }

    delete[] _pTot;
    _pTot = nullptr;
}

void IntegralForwardModel::createKappa(const frequenciesGroup &freq, const sources &src, const receivers &recv)
{
    _Kappa = new complexDataGrid2D *[freq.nFreq * src.nSrc * recv.nRecv];

    for(int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        _Kappa[i] = new complexDataGrid2D(_grid);
    }
}

void IntegralForwardModel::deleteKappa()
{
    for(int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        delete _Kappa[i];
    }

    delete[] _Kappa;
    _Kappa = nullptr;
}

complexDataGrid2D IntegralForwardModel::calcTotalField(
    const greensRect2DCpu &G, const dataGrid2D &chi, const complexDataGrid2D &p_init)
{
    assert(G.getGrid() == p_init.getGrid());

    Iter2 iter2 = _fmInput.iter2;

    complexDataGrid2D chi_p(_grid), chi_p_old(_grid);
    complexDataGrid2D dW(_grid), p_tot(_grid), f_rhs(_grid), matA_j(_grid);

    int n_cell = _grid.getNumberOfGridPoints();

    std::vector<complexDataGrid2D> phi;
    Matrix<std::complex<double>, Dynamic, Dynamic, ColMajor> matA;
    Matrix<std::complex<double>, Dynamic, 1, ColMajor> b_f_rhs;
    Matrix<std::complex<double>, Dynamic, 1, ColMajor> alpha;

    std::vector<std::complex<double>> matA_j_data, rhs_data;
    matA.conservativeResize(n_cell, 1);
    b_f_rhs.conservativeResize(n_cell, 1);

    double res = 0.0;

    chi_p_old.zero();

    p_tot = p_init;

    for(int it = 0; it < iter2.n; it++)
    {
        chi_p = p_tot * chi;

        dW = chi_p - chi_p_old;

        double dWNorm = dW.norm();
        double chi_pNorm = chi_p.norm();

        res = dWNorm / chi_pNorm;

        if(res < iter2.tolerance && it != 0)
        {
            if(true)
            {
                std::string itstring = std::to_string(it);
                std::string line_to_print = "Convergence after " + itstring + "iterations";
                L_(linfo) << line_to_print;
            }

            break;
        }

        if(iter2.calcAlpha)
        {
            phi.push_back(G.contractWithField(dW));
            f_rhs = G.contractWithField(chi * p_init);

            rhs_data = f_rhs.getData();

            matA_j = phi[it] - G.contractWithField(chi * phi[it]);
            matA_j_data = matA_j.getData();

            matA.conservativeResize(NoChange, it + 1);
            alpha.conservativeResize(it + 1);

            for(int i = 0; i < n_cell; i++)
            {
                matA(i, it) = matA_j_data[i];
                b_f_rhs(i) = rhs_data[i];
            }

            alpha = matA.jacobiSvd(ComputeThinU | ComputeThinV).solve(b_f_rhs);

            p_tot = p_init;

            for(int j = 0; j < it + 1; j++)
            {
                p_tot += alpha[j] * phi[j];
            }
        }
        else
        {
            p_tot += G.dot1(dW);
        }

        chi_p_old = chi_p;
        chi_p.zero();
    }

    if(res >= iter2.tolerance)
    {
        L_(linfo) << "No convergence after " << iter2.n << " iterations."
                  << "Res = " << res;
    }

    return p_tot;
}

void IntegralForwardModel::calculatePTot(const dataGrid2D &chiEst)
{
    assert(_Greens != nullptr);
    assert(_p0 != nullptr);

    int li;

    for(int i = 0; i < _freq.nFreq; i++)
    {
        li = i * _src.nSrc;

        L_(linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.nFreq << "freq";

        for(int j = 0; j < _src.nSrc; j++)
        {
            *_pTot[li + j] = calcTotalField(*_Greens[i], chiEst, *_p0[i][j]);
        }
    }
}

void IntegralForwardModel::calculatePData(const dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator) { applyKappa(chiEst, kOperator); }

void IntegralForwardModel::calculateKappa()
{
    int li, lj;

    for(int i = 0; i < _freq.nFreq; i++)
    {
        li = i * _recv.nRecv * _src.nSrc;

        for(int j = 0; j < _recv.nRecv; j++)
        {
            lj = j * _src.nSrc;

            for(int k = 0; k < _src.nSrc; k++)
            {
                *_Kappa[li + lj + k] = (*_Greens[i]->getReceiverCont(j)) * (*_pTot[i * _src.nSrc + k]);
            }
        }
    }
}

void IntegralForwardModel::mapDomainToSignal(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
{
    applyKappa(CurrentPressureFieldSerial, kOperator);
}

void IntegralForwardModel::applyKappa(const dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
{
    for(int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        kOperator[i] = dotProduct(*_Kappa[i], CurrentPressureFieldSerial);
    }
}

// void IntegralForwardModel::createKappaOperator(const complexDataGrid2D &CurrentPressureFieldComplexSerial, std::complex<double>* kOperator)
//{
//    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
//    {
//        kOperator[i] = summation( *_Kappa[i], CurrentPressureFieldComplexSerial);
//    }
//}

void IntegralForwardModel::getUpdateDirectionInformation(std::vector<std::complex<double>> &res, complexDataGrid2D &kRes)
{
    kRes.zero();

    complexDataGrid2D kDummy(_grid);

    for(int i = 0; i < _freq.nFreq * _recv.nRecv * _src.nSrc; i++)
    {
        kDummy = *_Kappa[i];
        kDummy.conjugate();
        kRes += kDummy * res[i];
    }
}

void IntegralForwardModel::getUpdateDirectionInformationMPI(
        std::vector<std::complex<double>> &res, complexDataGrid2D &kRes, const int offset, const int block_size)
{
    kRes.zero();

    complexDataGrid2D kDummy(_grid);

    for(int i = offset; i < offset + block_size; i++)
    {
        kDummy = *_Kappa[i];
        kDummy.conjugate();
        kRes += kDummy * res[i - offset];
    }
}
