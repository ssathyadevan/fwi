#include "FiniteDifferenceForwardModelParallelOpenCL.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "Helmholtz2D.h"
#include "log.h"

namespace fwi
{
    namespace forwardModels
    {
        FiniteDifferenceForwardModelOpenCL::FiniteDifferenceForwardModelOpenCL(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
            const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelOpenCLInput &fMInput)
            : _grid(grid)
            , _source(source)
            , _receiver(receiver)
            , _freq(freq)
            , _Greens()
            , _p0()
            , _pTot()
            , _kappa()
            , _fMInput(fMInput)
        {
            L_(io::linfo) << "Creating Greens function field...";
            createGreens();
            L_(io::linfo) << "Creating p0...";
            createP0();
            createPTot(freq, source);
            createKappa(freq, source, receiver);
        }

        FiniteDifferenceForwardModelOpenCL::~FiniteDifferenceForwardModelOpenCL()
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

        void FiniteDifferenceForwardModelOpenCL::createP0()
        {
            assert(_Greens != nullptr);
            assert(_p0 == nullptr);

            _p0 = new core::complexDataGrid2D **[_freq.count];

            for(int i = 0; i < _freq.count; i++)
            {
                _p0[i] = new core::complexDataGrid2D *[_source.count];

                for(int j = 0; j < _source.count; j++)
                {
                    _p0[i][j] = new core::complexDataGrid2D(_grid);
                    *_p0[i][j] = *(_Greens[i]->getReceiverCont(j)) / (_freq.k[i] * _freq.k[i] * _grid.getCellVolume());
                }
            }
        }

        void FiniteDifferenceForwardModelOpenCL::deleteP0()
        {
            for(int i = 0; i < _freq.count; i++)
            {
                for(int j = 0; j < _source.count; j++)
                {
                    delete _p0[i][j];
                }

                delete[] _p0[i];
            }

            delete[] _p0;
            _p0 = nullptr;
        }

        void FiniteDifferenceForwardModelOpenCL::createGreens()
        {
            _Greens = new core::greensRect2DCpu *[_freq.count];

            for(int i = 0; i < _freq.count; i++)
            {
                _Greens[i] = new core::greensRect2DCpu(_grid, core::greensFunctions::Helmholtz2D, _source, _receiver, _freq.k[i]);
            }
        }

        void FiniteDifferenceForwardModelOpenCL::deleteGreens()
        {
            for(int i = 0; i < _freq.count; i++)
            {
                delete _Greens[i];
            }

            delete[] _Greens;
            _Greens = nullptr;
        }

        void FiniteDifferenceForwardModelOpenCL::createPTot(const core::FrequenciesGroup &freq, const core::Sources &source)
        {
            _pTot = new core::complexDataGrid2D *[freq.count * source.count];

            int li;

            for(int i = 0; i < freq.count; i++)
            {
                li = i * source.count;

                for(int j = 0; j < source.count; j++)
                {
                    _pTot[li + j] = new core::complexDataGrid2D(*_p0[i][j]);
                }
            }
        }

        void FiniteDifferenceForwardModelOpenCL::deletePtot()
        {
            for(int i = 0; i < _freq.count * _source.count; i++)
            {
                delete _pTot[i];
            }

            delete[] _pTot;
            _pTot = nullptr;
        }

        void FiniteDifferenceForwardModelOpenCL::createKappa(const core::FrequenciesGroup &freq, const core::Sources &source, const core::Receivers &receiver)
        {
            _kappa = new core::complexDataGrid2D *[freq.count * source.count * receiver.count];

            for(int i = 0; i < freq.count * source.count * receiver.count; i++)
            {
                _kappa[i] = new core::complexDataGrid2D(_grid);
            }
        }

        void FiniteDifferenceForwardModelOpenCL::deleteKappa()
        {
            for(int i = 0; i < _freq.count * _source.count * _receiver.count; i++)
            {
                delete _kappa[i];
            }

            delete[] _kappa;
            _kappa = nullptr;
        }

        void FiniteDifferenceForwardModelOpenCL::calculatePTot(const core::dataGrid2D &chiEst)
        {
            assert(_Greens != nullptr);
            assert(_p0 != nullptr);

            int li;

            for(int i = 0; i < _freq.count; i++)
            {
                li = i * _source.count;

                Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _source, _freq.c0, chiEst, _fMInput);

                L_(io::linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.count << "freq";

                for(int j = 0; j < _source.count; j++)
                {
                    L_(io::linfo) << "Solving p_tot for source: (" << _source.xSrc[j][0] << "," << _source.xSrc[j][1] << ")";
                    *_pTot[li + j] = helmholtzFreq.solve(_source.xSrc[j], *_pTot[li + j]);
                }
            }
        }

        std::vector<std::complex<double>> FiniteDifferenceForwardModelOpenCL::calculatePressureField(const core::dataGrid2D &chiEst)
        {
            std::vector<std::complex<double>> kOperator(_freq.count * _source.count * _receiver.count);
            applyKappa(chiEst, kOperator);
            return kOperator;
        }

        void FiniteDifferenceForwardModelOpenCL::calculateKappa()
        {
            int li, lj;

            for(int i = 0; i < _freq.count; i++)
            {
                li = i * _receiver.count * _source.count;

                for(int j = 0; j < _receiver.count; j++)
                {
                    lj = j * _source.count;

                    for(int k = 0; k < _source.count; k++)
                    {
                        *_kappa[li + lj + k] = (*_Greens[i]->getReceiverCont(j)) * (*_pTot[i * _source.count + k]);
                    }
                }
            }
        }

        void FiniteDifferenceForwardModelOpenCL::applyKappa(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
        {
            for(int i = 0; i < _freq.count * _source.count * _receiver.count; i++)
            {
                kOperator[i] = dotProduct(*_kappa[i], CurrentPressureFieldSerial);
            }
        }

        void FiniteDifferenceForwardModelOpenCL::getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes)
        {
            int l_i, l_j;
            kRes.zero();
            core::complexDataGrid2D kDummy(_grid);

            for(int i = 0; i < _freq.count; i++)
            {
                l_i = i * _receiver.count * _source.count;
                for(int j = 0; j < _receiver.count; j++)
                {
                    l_j = j * _source.count;
                    for(int k = 0; k < _source.count; k++)
                    {
                        kDummy = *_kappa[l_i + l_j + k];
                        kDummy.conjugate();
                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
        }

        void FiniteDifferenceForwardModelOpenCL::getUpdateDirectionInformationMPI(
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

        void FiniteDifferenceForwardModelOpenCL::getResidualGradient(std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes)
        {
            int l_i, l_j;

            kRes.zero();

            core::complexDataGrid2D kDummy(_grid);

            for(int i = 0; i < _freq.count; i++)
            {
                l_i = i * _receiver.count * _source.count;

                for(int j = 0; j < _receiver.count; j++)
                {
                    l_j = j * _source.count;

                    for(int k = 0; k < _source.count; k++)
                    {
                        kDummy = *_kappa[l_i + l_j + k];

                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
        }
    }   // namespace forwardModels
}   // namespace fwi
