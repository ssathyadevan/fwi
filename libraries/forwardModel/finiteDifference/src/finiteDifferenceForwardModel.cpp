#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "helmholtz2D.h"
#include "log.h"

finiteDifferenceForwardModel::finiteDifferenceForwardModel(const core::grid2D &grid, const core::sources &src, const core::receivers &recv,
    const core::frequenciesGroup &freq, const finiteDifferenceForwardModelInput &fMInput)
    : forwardModelInterface(grid, src, recv, freq)
    , _Greens()
    , _p0()
    , _pTot()
    , _kappa()
    , _fMInput(fMInput)
{
    L_(linfo) << "Creating Greens function field...";
    createGreens();
    L_(linfo) << "Creating p0...";
    createP0();
    createPTot(freq, src);
    createKappa(freq, src, recv);
}

finiteDifferenceForwardModel::~finiteDifferenceForwardModel()
{
    if(_Greens != nullptr)
        this->deleteGreens();

    if(_p0 != nullptr)
        this->deleteP0();

    if(_pTot != nullptr)
        this->deletePtot();

    if(_kappa != nullptr)
        this->deleteKappa();
}

void finiteDifferenceForwardModel::createP0()
{
    assert(_Greens != nullptr);
    assert(_p0 == nullptr);

    _p0 = new core::complexDataGrid2D **[_freq.nFreq];

    for(int i = 0; i < _freq.nFreq; i++)
    {
        _p0[i] = new core::complexDataGrid2D *[_src.nSrc];

        for(int j = 0; j < _src.nSrc; j++)
        {
            _p0[i][j] = new core::complexDataGrid2D(_grid);
            *_p0[i][j] = *(_Greens[i]->getReceiverCont(j)) / (_freq.k[i] * _freq.k[i] * _grid.getCellVolume());
        }
    }
}

void finiteDifferenceForwardModel::deleteP0()
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

void finiteDifferenceForwardModel::createGreens()
{
    _Greens = new core::greensRect2DCpu *[_freq.nFreq];

    for(int i = 0; i < _freq.nFreq; i++)
    {
        _Greens[i] = new core::greensRect2DCpu(_grid, core::greensFunctions::Helmholtz2D, _src, _recv, _freq.k[i]);
    }
}

void finiteDifferenceForwardModel::deleteGreens()
{
    for(int i = 0; i < _freq.nFreq; i++)
    {
        delete _Greens[i];
    }

    delete[] _Greens;
    _Greens = nullptr;
}

void finiteDifferenceForwardModel::createPTot(const core::frequenciesGroup &freq, const core::sources &src)
{
    _pTot = new core::complexDataGrid2D *[freq.nFreq * src.nSrc];

    int li;

    for(int i = 0; i < freq.nFreq; i++)
    {
        li = i * src.nSrc;

        for(int j = 0; j < src.nSrc; j++)
        {
            _pTot[li + j] = new core::complexDataGrid2D(*_p0[i][j]);
        }
    }
}

void finiteDifferenceForwardModel::deletePtot()
{
    for(int i = 0; i < _freq.nFreq * _src.nSrc; i++)
    {
        delete _pTot[i];
    }

    delete[] _pTot;
    _pTot = nullptr;
}

void finiteDifferenceForwardModel::createKappa(const core::frequenciesGroup &freq, const core::sources &src, const core::receivers &recv)
{
    _kappa = new core::complexDataGrid2D *[freq.nFreq * src.nSrc * recv.nRecv];

    for(int i = 0; i < freq.nFreq * src.nSrc * recv.nRecv; i++)
    {
        _kappa[i] = new core::complexDataGrid2D(_grid);
    }
}

void finiteDifferenceForwardModel::deleteKappa()
{
    for(int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        delete _kappa[i];
    }

    delete[] _kappa;
    _kappa = nullptr;
}

void finiteDifferenceForwardModel::calculatePTot(const core::dataGrid2D &chiEst)
{
    assert(_Greens != nullptr);
    assert(_p0 != nullptr);

    int li;

    for(int i = 0; i < _freq.nFreq; i++)
    {
        li = i * _src.nSrc;

        Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _src, _freq.c0, chiEst, _fMInput);

        L_(linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.nFreq << "freq";

        for(int j = 0; j < _src.nSrc; j++)
        {
            L_(linfo) << "Solving p_tot for source: (" << _src.xSrc[j][0] << "," << _src.xSrc[j][1] << ")";
            *_pTot[li + j] = helmholtzFreq.solve(_src.xSrc[j], *_pTot[li + j]);
        }
    }
}

void finiteDifferenceForwardModel::calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator)
{
    applyKappa(chiEst, kOperator);
}

void finiteDifferenceForwardModel::calculateKappa()
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
                *_kappa[li + lj + k] = (*_Greens[i]->getReceiverCont(j)) * (*_pTot[i * _src.nSrc + k]);
            }
        }
    }
}

void finiteDifferenceForwardModel::mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
{
    applyKappa(CurrentPressureFieldSerial, kOperator);
}

void finiteDifferenceForwardModel::applyKappa(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
{
    for(int i = 0; i < _freq.nFreq * _src.nSrc * _recv.nRecv; i++)
    {
        kOperator[i] = dotProduct(*_kappa[i], CurrentPressureFieldSerial);
    }
}

void finiteDifferenceForwardModel::getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes)
{
    int l_i, l_j;
    kRes.zero();
    core::complexDataGrid2D kDummy(_grid);

    for(int i = 0; i < _freq.nFreq; i++)
    {
        l_i = i * _recv.nRecv * _src.nSrc;
        for(int j = 0; j < _recv.nRecv; j++)
        {
            l_j = j * _src.nSrc;
            for(int k = 0; k < _src.nSrc; k++)
            {
                kDummy = *_kappa[l_i + l_j + k];
                kDummy.conjugate();
                kRes += kDummy * res[l_i + l_j + k];
            }
        }
    }
}

void finiteDifferenceForwardModel::getUpdateDirectionInformationMPI(
    std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes, const int offset, const int block_size)
{
    kRes.zero();

    core::complexDataGrid2D kDummy(_grid);

    for(int i = offset; i < offset + block_size; i++)
    {
        kDummy = *_kappa[i];
        kDummy.conjugate();
        kRes += kDummy * res[i - offset];
    }
}

void finiteDifferenceForwardModel::getResidualGradient(std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes)
{
    int l_i, l_j;

    kRes.zero();

    core::complexDataGrid2D kDummy(_grid);

    for(int i = 0; i < _freq.nFreq; i++)
    {
        l_i = i * _recv.nRecv * _src.nSrc;

        for(int j = 0; j < _recv.nRecv; j++)
        {
            l_j = j * _src.nSrc;

            for(int k = 0; k < _src.nSrc; k++)
            {
                kDummy = *_kappa[l_i + l_j + k];

                kRes += kDummy * res[l_i + l_j + k];
            }
        }
    }
}
