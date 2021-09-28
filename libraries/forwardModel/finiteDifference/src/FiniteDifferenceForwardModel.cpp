#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "Helmholtz2D.h"
#include "log.h"

namespace fwi
{
    namespace forwardModels
    {
        FiniteDifferenceForwardModel::FiniteDifferenceForwardModel(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
            const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fMInput)
            : _grid(grid)
            , _source(source)
            , _receiver(receiver)
            , _freq(freq)
            , _Greens()
            , _vpTot()
            , _vkappa()
            , _fMInput(fMInput)
        {
            L_(io::linfo) << "Creating Greens function field...";
            createGreens();
            L_(io::linfo) << "Creating initial Ptot";
            createPTot(freq, source);
            createKappa(freq, source, receiver);
            calculateKappa();
        }

        FiniteDifferenceForwardModel::~FiniteDifferenceForwardModel()
        {
            if(_Greens != nullptr)
                this->deleteGreens();
        }

        void FiniteDifferenceForwardModel::createGreens()
        {
            _Greens = new core::greensRect2DCpu *[_freq.count];

            for(int i = 0; i < _freq.count; i++)
            {
                _Greens[i] = new core::greensRect2DCpu(_grid, core::greensFunctions::Helmholtz2D, _source, _receiver, _freq.k[i]);
            }
        }

        void FiniteDifferenceForwardModel::deleteGreens()
        {
            for(int i = 0; i < _freq.count; i++)
            {
                delete _Greens[i];
            }

            delete[] _Greens;
            _Greens = nullptr;
        }

        void FiniteDifferenceForwardModel::createPTot(const core::FrequenciesGroup &freq, const core::Sources &source)
        {
            int li;

            for(int i = 0; i < freq.count; i++)
            {
                li = i * source.count;

                for(int j = 0; j < source.count; j++)
                {
                    _vpTot.push_back(core::dataGrid2D<std::complex<double>> (*_Greens[i]->getReceiverCont(j) / (_freq.k[i] * _freq.k[i] * _grid.getCellVolume())));
                }
            }
        }

        void FiniteDifferenceForwardModel::createKappa(const core::FrequenciesGroup &freq, const core::Sources &source, const core::Receivers &receiver)
        {
            for(int i = 0; i < freq.count * source.count * receiver.count; i++)
            {
                _vkappa.push_back(core::dataGrid2D<std::complex<double>> (_grid));
            }
        }

        void FiniteDifferenceForwardModel::deleteKappa()
        {

        }

        void FiniteDifferenceForwardModel::calculatePTot(const core::dataGrid2D<double> &chiEst)
        {
            assert(_Greens != nullptr);

            int li;

            for(int i = 0; i < _freq.count; i++)
            {
                li = i * _source.count;

                Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _source, _freq.c0, chiEst, _fMInput);

                L_(io::linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.count << "freq";

                for(int j = 0; j < _source.count; j++)
                {
                    L_(io::linfo) << "Solving p_tot for source: (" << _source.xSrc[j][0] << "," << _source.xSrc[j][1] << ")";
                    _vpTot[li + j] = helmholtzFreq.solve(_source.xSrc[j], _vpTot[li + j]);
                }
            }
        }

        std::vector<std::complex<double>> FiniteDifferenceForwardModel::calculatePressureField(const core::dataGrid2D<double> &chiEst)
        {
            std::vector<std::complex<double>> kOperator(_freq.count * _source.count * _receiver.count);
            applyKappa(chiEst, kOperator);
            return kOperator;
        }

        void FiniteDifferenceForwardModel::calculateKappa()
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
                        _vkappa[li + lj + k] = (*_Greens[i]->getReceiverCont(j)) * (_vpTot[i * _source.count + k]);
                    }
                }
            }
        }

        void FiniteDifferenceForwardModel::applyKappa(const core::dataGrid2D<double> &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
        {
            for(int i = 0; i < _freq.count * _source.count * _receiver.count; i++)
            {
                kOperator[i] = dotProduct(_vkappa[i], CurrentPressureFieldSerial);
            }
        }

        void FiniteDifferenceForwardModel::getUpdateDirectionInformation(
            const std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes)
        {
            int l_i, l_j;
            kRes.zero();
            core::dataGrid2D<std::complex<double>> kDummy(_grid);

            for(int i = 0; i < _freq.count; i++)
            {
                l_i = i * _receiver.count * _source.count;
                for(int j = 0; j < _receiver.count; j++)
                {
                    l_j = j * _source.count;
                    for(int k = 0; k < _source.count; k++)
                    {
                        kDummy = _vkappa[l_i + l_j + k];
                        kDummy.conjugate();
                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
        }

        void FiniteDifferenceForwardModel::getUpdateDirectionInformationMPI(
            std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes, const int offset, const int block_size)
        {
            kRes.zero();

            core::dataGrid2D<std::complex<double>> kDummy(_grid);

            for(int i = offset; i < offset + block_size; i++)
            {
                kDummy = _vkappa[i];
                kDummy.conjugate();
                kRes += kDummy * res[i - offset];
            }
        }

        void FiniteDifferenceForwardModel::getResidualGradient(std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes)
        {
            int l_i, l_j;

            kRes.zero();

            core::dataGrid2D<std::complex<double>> kDummy(_grid);

            for(int i = 0; i < _freq.count; i++)
            {
                l_i = i * _receiver.count * _source.count;

                for(int j = 0; j < _receiver.count; j++)
                {
                    l_j = j * _source.count;

                    for(int k = 0; k < _source.count; k++)
                    {
                        kDummy = _vkappa[l_i + l_j + k];
                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
        }
    }   // namespace forwardModels
}   // namespace fwi
