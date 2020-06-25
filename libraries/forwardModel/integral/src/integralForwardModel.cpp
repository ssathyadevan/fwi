#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"

namespace fwi
{
    namespace forwardModels
    {
        IntegralForwardModel::IntegralForwardModel(const core::grid2D &grid, const core::Sources &source, const core::Receivers &receiver,
            const core::FrequenciesGroup &freq, const integralForwardModelInput &fmInput)
            : forwardModelInterface(grid, source, receiver, freq)
            , _Greens()
            , _p0()
            , _pTot()
            , _Kappa()
            , _fmInput(fmInput)
        {
            L_(io::linfo) << "Creating Greens function field...";
            createGreens();
            L_(io::linfo) << "Creating p0...";
            createP0();
            createPTot(freq, source);
            createKappa(freq, source, receiver);
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

            _p0 = new core::complexDataGrid2D **[_freq.nFreq];

            for(int i = 0; i < _freq.nFreq; i++)
            {
                _p0[i] = new core::complexDataGrid2D *[_source.count];

                for(int j = 0; j < _source.count; j++)
                {
                    _p0[i][j] = new core::complexDataGrid2D(_grid);
                    *_p0[i][j] = *(_Greens[i]->getReceiverCont(j)) / (_freq.k[i] * _freq.k[i] * _grid.getCellVolume());
                }
            }
        }

        void IntegralForwardModel::deleteP0()
        {
            for(int i = 0; i < _freq.nFreq; i++)
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

        void IntegralForwardModel::createGreens()
        {
            _Greens = new core::greensRect2DCpu *[_freq.nFreq];

            for(int i = 0; i < _freq.nFreq; i++)
            {
                _Greens[i] = new core::greensRect2DCpu(_grid, core::greensFunctions::Helmholtz2D, _source, _receiver, _freq.k[i]);
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

        void IntegralForwardModel::createPTot(const core::FrequenciesGroup &freq, const core::Sources &source)
        {
            _pTot = new core::complexDataGrid2D *[freq.nFreq * source.count];

            int li;

            for(int i = 0; i < freq.nFreq; i++)
            {
                li = i * source.count;

                for(int j = 0; j < source.count; j++)
                {
                    _pTot[li + j] = new core::complexDataGrid2D(*_p0[i][j]);
                }
            }
        }

        void IntegralForwardModel::deletePtot()
        {
            for(int i = 0; i < _freq.nFreq * _source.count; i++)
            {
                delete _pTot[i];
            }

            delete[] _pTot;
            _pTot = nullptr;
        }

        void IntegralForwardModel::createKappa(const core::FrequenciesGroup &freq, const core::Sources &source, const core::Receivers &receiver)
        {
            _Kappa = new core::complexDataGrid2D *[freq.nFreq * source.count * receiver.count];

            for(int i = 0; i < freq.nFreq * source.count * receiver.count; i++)
            {
                _Kappa[i] = new core::complexDataGrid2D(_grid);
            }
        }

        void IntegralForwardModel::deleteKappa()
        {
            for(int i = 0; i < _freq.nFreq * _source.count * _receiver.count; i++)
            {
                delete _Kappa[i];
            }

            delete[] _Kappa;
            _Kappa = nullptr;
        }

        core::complexDataGrid2D IntegralForwardModel::calcTotalField(
            const core::greensRect2DCpu &G, const core::dataGrid2D &chi, const core::complexDataGrid2D &p_init)
        {
            assert(G.getGrid() == p_init.getGrid());

            core::complexDataGrid2D chi_p(_grid), chi_p_old(_grid);
            core::complexDataGrid2D dW(_grid), p_tot(_grid), f_rhs(_grid), matA_j(_grid);

            int n_cell = _grid.getNumberOfGridPoints();

            std::vector<core::complexDataGrid2D> phi;
            Matrix<std::complex<double>, Dynamic, Dynamic, ColMajor> matA;
            Matrix<std::complex<double>, Dynamic, 1, ColMajor> b_f_rhs;
            Matrix<std::complex<double>, Dynamic, 1, ColMajor> alpha;

            std::vector<std::complex<double>> matA_j_data, rhs_data;
            matA.conservativeResize(n_cell, 1);
            b_f_rhs.conservativeResize(n_cell, 1);

            double res = 0.0;

            chi_p_old.zero();

            p_tot = p_init;

            for(int it = 0; it < _fmInput.nrOfIterations; it++)
            {
                chi_p = p_tot * chi;

                dW = chi_p - chi_p_old;

                double dWNorm = dW.norm();
                double chi_pNorm = chi_p.norm();

                res = dWNorm / chi_pNorm;

                if(res < _fmInput.tolerance && it != 0)
                {
                    if(true)
                    {
                        std::string itstring = std::to_string(it);
                        std::string line_to_print = "Convergence after " + itstring + "iterations";
                        L_(io::linfo) << line_to_print;
                    }

                    break;
                }

                if(_fmInput.calcAlpha)
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

            if(res >= _fmInput.tolerance)
            {
                L_(io::linfo) << "No convergence after " << _fmInput.nrOfIterations << " iterations."
                              << "Res = " << res;
            }

            return p_tot;
        }

        void IntegralForwardModel::calculatePTot(const core::dataGrid2D &chiEst)
        {
            assert(_Greens != nullptr);
            assert(_p0 != nullptr);

            int li;

            for(int i = 0; i < _freq.nFreq; i++)
            {
                li = i * _source.count;

                L_(io::linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.nFreq << "freq";

                for(int j = 0; j < _source.count; j++)
                {
                    *_pTot[li + j] = calcTotalField(*_Greens[i], chiEst, *_p0[i][j]);
                }
            }
        }

        void IntegralForwardModel::calculatePData(const core::dataGrid2D &chiEst, std::vector<std::complex<double>> &kOperator)
        {
            applyKappa(chiEst, kOperator);
        }

        void IntegralForwardModel::calculateKappa()
        {
            int li, lj;

            for(int i = 0; i < _freq.nFreq; i++)
            {
                li = i * _receiver.count * _source.count;

                for(int j = 0; j < _receiver.count; j++)
                {
                    lj = j * _source.count;

                    for(int k = 0; k < _source.count; k++)
                    {
                        *_Kappa[li + lj + k] = (*_Greens[i]->getReceiverCont(j)) * (*_pTot[i * _source.count + k]);
                    }
                }
            }
        }

        void IntegralForwardModel::mapDomainToSignal(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
        {
            applyKappa(CurrentPressureFieldSerial, kOperator);
        }

        void IntegralForwardModel::applyKappa(const core::dataGrid2D &CurrentPressureFieldSerial, std::vector<std::complex<double>> &kOperator)
        {
            for(int i = 0; i < _freq.nFreq * _source.count * _receiver.count; i++)
            {
                kOperator[i] = dotProduct(*_Kappa[i], CurrentPressureFieldSerial);
            }
        }

        void IntegralForwardModel::getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes)
        {
            int l_i, l_j;
            kRes.zero();
            core::complexDataGrid2D kDummy(_grid);

            for(int i = 0; i < _freq.nFreq; i++)
            {
                l_i = i * _receiver.count * _source.count;
                for(int j = 0; j < _receiver.count; j++)
                {
                    l_j = j * _source.count;
                    for(int k = 0; k < _source.count; k++)
                    {
                        kDummy = *_Kappa[l_i + l_j + k];
                        kDummy.conjugate();
                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
        }

        void IntegralForwardModel::getUpdateDirectionInformationMPI(
            std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes, const int offset, const int block_size)
        {
            kRes.zero();

            core::complexDataGrid2D kDummy(_grid);

            for(int i = offset; i < offset + block_size; i++)
            {
                kDummy = *_Kappa[i];
                kDummy.conjugate();
                kRes += kDummy * res[i - offset];
            }
        }
    }   // namespace forwardModels
}   // namespace fwi
