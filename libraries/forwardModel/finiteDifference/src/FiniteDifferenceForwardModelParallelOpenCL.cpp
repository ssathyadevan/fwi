

#include "FiniteDifferenceForwardModelParallelOpenCL.h"
#include "Helmholtz2D.h"
#include <complex>
#include <omp.h>
#include <vector>

namespace fwi
{
    namespace forwardModels
    {
        FiniteDifferenceForwardModelParallelOpenCL::FiniteDifferenceForwardModelParallelOpenCL(const core::grid2D &grid, const core::Sources &source,
            const core::Receivers &receiver, const core::FrequenciesGroup &freq, const finiteDifferenceForwardModelInput &fmInput)
            : FiniteDifferenceForwardModel(grid, source, receiver, freq, fmInput)
        {
            // L_(io::linfo) << "OpenMP number of threads: " << omp_get_max_threads();
        }
        void FiniteDifferenceForwardModelParallelOpenCL::calculateKappa()
        {
            cl::Kernel kernKappa(program, "calculateKappa");
            cl::CommandQueue queue(context, device, 0, &err); 

            // cl::Buffer bufFreq(context, CL_MEM_READ_ONLY | CL_MEM_HOST_READ_ONLY | CL_MEM_USE_HOST_PTR, sizeof(float)*_freq.count, &_freq.count);
            // err = kern1.setArg(0, buf0);

            // err = queue.enqueueNDRangeKernel(kern1, cl::NullRange, cl::NDRange(_freq.count,_receiver.count,_source.count));
            // err = queue.enqueueReadBuffer(bufout, CL_TRUE, 0, sizeof(float)*vec.size(), vec.data());

            int li, lj, k;

#pragma omp parallel for private(li, lj, k)
            for(int i = 0; i < _freq.count; i++)
            {
                li = i * _receiver.count * _source.count;

                for(int j = 0; j < _receiver.count; j++)
                {
                    lj = j * _source.count;

                    for(k = 0; k < _source.count; k++)
                    {
                        *_kappa[li + lj + k] = (*_Greens[i]->getReceiverCont(j)) * (*_pTot[i * _source.count + k]);
                    }
                }
            }
        }

        void FiniteDifferenceForwardModelParallelOpenCL::calculatePTot(const core::dataGrid2D &chiEst)
        {
            assert(_Greens != nullptr);
            assert(_p0 != nullptr);

            auto copyPTot = _pTot;

            int li, i, j;
#pragma omp parallel for private(li, i, j)
            for(i = 0; i < _freq.count; i++)
            {
                li = i * _source.count;

                Helmholtz2D helmholtzFreq(_grid, _freq.freq[i], _source, _freq.c0, chiEst, _fMInput);

                L_(io::linfo) << "Creating this->p_tot for " << i + 1 << "/ " << _freq.count << "freq";

                for(j = 0; j < _source.count; j++)
                {
                    // Review: Remove logging statements?
                    // L_(io::linfo) << "Solving P_tot for source: (" << _source.xSrc[j][0] << "," << _source.xSrc[j][1] << ")"; 

                    *_pTot[li + j] = helmholtzFreq.solve(_source.xSrc[j], *copyPTot[li + j]);
                }
            }
        }

        std::vector<std::complex<double>> FiniteDifferenceForwardModelParallelOpenCL::calculatePressureField(const core::dataGrid2D &chiEst)
        {
            std::vector<std::complex<double>> kOperator(_freq.count * _source.count * _receiver.count);
#pragma omp parallel for
            for(int i = 0; i < _freq.count * _source.count * _receiver.count; i++)
            {
                kOperator[i] = dotProduct(*_kappa[i], chiEst);
            }
            return kOperator;
        }

        void FiniteDifferenceForwardModelParallelOpenCL::getUpdateDirectionInformation(const std::vector<std::complex<double>> &res, core::complexDataGrid2D &kRes)
        {
#pragma omp declare reduction(addition : class core::complexDataGrid2D : omp_out += omp_in) initializer(omp_priv(omp_orig))
#pragma omp declare reduction(multiplication : class core::complexDataGrid2D : omp_out *omp_in) initializer(omp_priv(omp_orig))
            int l_i, l_j, k;
            kRes.zero();
            core::complexDataGrid2D kDummy(_grid);
#pragma omp parallel for reduction(addition : kRes) reduction(multiplication : kDummy) private(l_i, l_j, k)
            for(int i = 0; i < _freq.count; i++)
            {
                l_i = i * _receiver.count * _source.count;
                for(int j = 0; j < _receiver.count; j++)
                {
                    l_j = j * _source.count;
                    for(k = 0; k < _source.count; k++)
                    {
                        kDummy = *_kappa[l_i + l_j + k];
                        kDummy.conjugate();
                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
            // namespace forwardModels
        }
    }   // namespace forwardModels
    void OpenCLParallelized::setProgram(cl::Program prog){
            program = prog;
        }
    void OpenCLParallelized::setContext(cl::Context con){
        context = con;
    }
    void OpenCLParallelized::setDevice(cl::Device dev){
        device = dev;
    }
}   // namespace fwi

cl::Program fwi::OpenCLParallelized::program;
cl::Context fwi::OpenCLParallelized::context;
cl::Device fwi::OpenCLParallelized::device;