#include "finiteDifferenceForwardModel.h"
#include "helmholtz2D.h"

FiniteDifferenceForwardModel::FiniteDifferenceForwardModel(const grid2D &grid, const sources &src, const receivers &recv,
                           const frequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput)
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

FiniteDifferenceForwardModel::~FiniteDifferenceForwardModel()
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

void FiniteDifferenceForwardModel::createP0()
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

void FiniteDifferenceForwardModel::deleteP0()
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

void FiniteDifferenceForwardModel::createGreens()
{
    _Greens = new Greens_rect_2D_cpu*[_freq.nFreq];

    for (int i = 0; i < _freq.nFreq; i++)
    {
        _Greens[i] = new Greens_rect_2D_cpu(_grid, GreensFunctions::Helmholtz2D, _src, _recv, _freq.k[i]);
    }
}

void FiniteDifferenceForwardModel::deleteGreens()
{
    for (int i=0; i < _freq.nFreq; i++)
    {
        delete _Greens[i];
    }

    delete[] _Greens;
    _Greens = nullptr;
}

void FiniteDifferenceForwardModel::createPTot(const frequenciesGroup &freq, const sources &src)
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

void FiniteDifferenceForwardModel::deletePtot()
{
    for (int i = 0; i < _freq.nFreq * _src.nSrc; i++)
    {
        delete _pTot[i];
    }

    delete[] _pTot;
    _pTot = nullptr;
}

void FiniteDifferenceForwardModel::createKappa(const frequenciesGroup &freq, const sources &src, const receivers &recv)
{
    _Kappa = new pressureFieldComplexSerial*[freq.nFreq * src.nSrc * recv.nRecv];

    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        _Kappa[i] = new pressureFieldComplexSerial(_grid);
    }
}

void FiniteDifferenceForwardModel::deleteKappa()
{
    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        delete _Kappa[i];
    }

    delete[] _Kappa;
    _Kappa = nullptr;
}

void FiniteDifferenceForwardModel::calculatePTot(const pressureFieldSerial &chiEst)
{
    assert(_Greens  != nullptr);
    assert(_p0      != nullptr);

    int li;

    for (int i = 0; i < _freq.nFreq; i++)
    {
        li = i * _src.nSrc;

        Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _src, _freq.c_0, chiEst, _fmInput);

        std::cout << "  " << std::endl;
        std::cout << "Creating this->p_tot for " << i+1 << "/ " << _freq.nFreq << "freq" << std::endl;
        std::cout << "  " << std::endl;

        for (int j = 0; j < _src.nSrc; j++)
        {
            std::cout << "Solving p_tot for source: (" << _src.xSrc[j][0] << "," << _src.xSrc[j][1] << ")" << std::endl;
            *_pTot[li + j] = helmholtzFreq.solve(_src.xSrc[j], *_pTot[li + j]);
        }
    }
    std::cout << " " << std::endl;
}

void FiniteDifferenceForwardModel::calculatePData(const pressureFieldSerial &chiEst, std::complex<double> *kOperator)
{
    applyKappa(chiEst, kOperator);
}

void FiniteDifferenceForwardModel::calculateKappa()
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

void FiniteDifferenceForwardModel::mapDomainToSignal(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double> *kOperator)
{
    applyKappa(CurrentPressureFieldSerial, kOperator);
}

void FiniteDifferenceForwardModel::applyKappa(const pressureFieldSerial &CurrentPressureFieldSerial, std::complex<double> *kOperator)
{
    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {       
        kOperator[i] = Summation( *_Kappa[i], CurrentPressureFieldSerial);
    }
}

void FiniteDifferenceForwardModel::getUpdateDirectionInformation(std::complex<double>* res, pressureFieldComplexSerial &kRes)
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

void FiniteDifferenceForwardModel::getResidualGradient(std::complex<double>* res, pressureFieldComplexSerial &kRes)
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

                kRes += kDummy * res[l_i + l_j + k];
            }
        }
    }
}

