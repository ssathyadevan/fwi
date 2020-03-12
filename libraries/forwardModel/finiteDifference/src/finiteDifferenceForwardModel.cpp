#include "finiteDifferenceForwardModel.h"
#include "helmholtz2D.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "log.h"

FiniteDifferenceForwardModel::FiniteDifferenceForwardModel(const Grid2D &grid, const Sources &src, const Receivers &recv,
                                                           const FrequenciesGroup &freq, const FiniteDifferenceForwardModelInput &fmInput)
    : ForwardModelInterface(grid, src, recv, freq),
      _Greens(), _p0(), _pTot(), _Kappa(), _fmInput(fmInput)
{
    L_(linfo) << "Creating Greens function field..." ;
    createGreens();
    L_(linfo) << "Creating p0..." ;
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

    _p0 = new PressureFieldComplexSerial **[_freq.nFreq];

    for (int i = 0; i < _freq.nFreq; i++)
    {
        _p0[i] = new PressureFieldComplexSerial *[_src.nSrc];

        for (int j = 0; j < _src.nSrc; j++)
        {
            _p0[i][j] = new PressureFieldComplexSerial(_grid);
            *_p0[i][j] = *(_Greens[i]->GetReceiverCont(j)) / (_freq.k[i] * _freq.k[i] * _grid.GetCellVolume());
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
    _Greens = new Greens_rect_2D_cpu *[_freq.nFreq];

    for (int i = 0; i < _freq.nFreq; i++)
    {
        _Greens[i] = new Greens_rect_2D_cpu(_grid, GreensFunctions::Helmholtz2D, _src, _recv, _freq.k[i]);
    }
}

void FiniteDifferenceForwardModel::deleteGreens()
{
    for (int i = 0; i < _freq.nFreq; i++)
    {
        delete _Greens[i];
    }

    delete[] _Greens;
    _Greens = nullptr;
}

void FiniteDifferenceForwardModel::createPTot(const FrequenciesGroup &freq, const Sources &src)
{
    _pTot = new PressureFieldComplexSerial *[freq.nFreq * src.nSrc];

    int li;

    for (int i = 0; i < freq.nFreq; i++)
    {
        li = i * src.nSrc;

        for (int j = 0; j < src.nSrc; j++)
        {
            _pTot[li + j] = new PressureFieldComplexSerial(*_p0[i][j]);
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

void FiniteDifferenceForwardModel::createKappa(const FrequenciesGroup &freq, const Sources &src, const Receivers &recv)
{
    _Kappa = new PressureFieldComplexSerial *[freq.nFreq * src.nSrc * recv.nRecv];

    for (int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        _Kappa[i] = new PressureFieldComplexSerial(_grid);
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

void FiniteDifferenceForwardModel::calculatePTot(const PressureFieldSerial &chiEst)
{
    assert(_Greens != nullptr);
    assert(_p0 != nullptr);

    int li;

    for (int i = 0; i < _freq.nFreq; i++)
    {
        li = i * _src.nSrc;

        Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _src, _freq.c_0, chiEst, _fmInput);

        L_(linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.nFreq << "freq" ;

        for (int j = 0; j < _src.nSrc; j++)
        {
            L_(linfo) << "Solving p_tot for source: (" << _src.xSrc[j][0] << "," << _src.xSrc[j][1] << ")" ;
            *_pTot[li + j] = helmholtzFreq.solve(_src.xSrc[j], *_pTot[li + j]);
        }
    }
}

void FiniteDifferenceForwardModel::calculatePData(const PressureFieldSerial &chiEst, std::vector<std::complex<double>> &kOperator)
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

            for (int k = 0; k < _src.nSrc; k++)
            {
                *_Kappa[li + lj + k] = (*_Greens[i]->GetReceiverCont(j)) * (*_pTot[i * _src.nSrc + k]);
            }
        }
    }
}

void FiniteDifferenceForwardModel::mapDomainToSignal(const PressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
{
    applyKappa(CurrentPressureFieldSerial, kOperator);
}

void FiniteDifferenceForwardModel::applyKappa(const PressureFieldSerial &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
{
    for (int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        kOperator[i] = DotProduct(*_Kappa[i], CurrentPressureFieldSerial);
    }
}

void FiniteDifferenceForwardModel::getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, PressureFieldComplexSerial &kRes)
{
    int l_i, l_j;

    kRes.Zero();

    PressureFieldComplexSerial kDummy(_grid);

    for (int i = 0; i < _freq.nFreq; i++)
    {
        l_i = i * _recv.nRecv * _src.nSrc;

        for (int j = 0; j < _recv.nRecv; j++)
        {
            l_j = j * _src.nSrc;

            for (int k = 0; k < _src.nSrc; k++)
            {
                kDummy = *_Kappa[l_i + l_j + k];
                kDummy.Conjugate();

                kRes += kDummy * res[l_i + l_j + k];
            }
        }
    }
}

void FiniteDifferenceForwardModel::getUpdateDirectionInformationMPI(std::vector<std::complex<double>> &res, PressureFieldComplexSerial &kRes, const int offset, const int block_size) {
    kRes.Zero();

    PressureFieldComplexSerial kDummy(_grid);

    for (int i = offset; i < offset + block_size; i++)
    {
        kDummy = *_Kappa[i];
        kDummy.Conjugate();
        kRes += kDummy * res[i - offset];
    }
    
}

void FiniteDifferenceForwardModel::getResidualGradient(std::vector<std::complex<double>> &res, PressureFieldComplexSerial &kRes)
{
    int l_i, l_j;

    kRes.Zero();

    PressureFieldComplexSerial kDummy(_grid);

    for (int i = 0; i < _freq.nFreq; i++)
    {
        l_i = i * _recv.nRecv * _src.nSrc;

        for (int j = 0; j < _recv.nRecv; j++)
        {
            l_j = j * _src.nSrc;

            for (int k = 0; k < _src.nSrc; k++)
            {
                kDummy = *_Kappa[l_i + l_j + k];

                kRes += kDummy * res[l_i + l_j + k];
            }
        }
    }
}
