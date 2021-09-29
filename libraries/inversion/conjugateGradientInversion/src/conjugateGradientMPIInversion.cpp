#ifdef MPI
#include "conjugateGradientMPIInversion.h"
#include "CommonVectorOperations.h"
#include "log.h"
#include "progressBar.h"
#include <boost/mpi.hpp>
#include <boost/serialization/complex.hpp>
#include <boost/serialization/vector.hpp>
#include <iostream>
#include <memory>

namespace mpi = boost::mpi;

namespace fwi
{
    namespace inversionMethods
    {
        ConjugateGradientMPIInversion::ConjugateGradientMPIInversion(const core::CostFunctionCalculator &costCalculator,
            forwardModels::ForwardModelInterface *forwardModel, const ConjugateGradientInversionInput &invInput)
            : ConjugateGradientInversion(costCalculator, forwardModel, invInput)
        {
        }

        ConjugateGradientMPIInversion::~ConjugateGradientMPIInversion() {}

        void ConjugateGradientMPIInversion::determineStartStopMPI(int &start, int &stop, const int &freq)
        {
            mpi::communicator world;
            int devide = freq / world.size();
            int rest = freq % world.size();
            start = world.rank() * devide;
            stop = (world.rank() + 1) * devide;

            for(int j = 0; j < world.rank(); j++)
            {
                if(j < rest)
                {
                    start += j - 1 == 0 ? 0 : 1;
                    stop += 1;
                }
            }
        }

        void ConjugateGradientMPIInversion::getUpdateDirectionInformation(
            const std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes)
        {
            mpi::communicator world;
            for(int i = 1; i < world.size(); i++)
            {
                world.send(i, 2, res);
            }

            int start, stop;
            determineStartStopMPI(start, stop, _frequencies.count);

            int l_i, l_j;
            kRes.zero();
            core::dataGrid2D<std::complex<double>> kDummy(_grid);
            auto kappa = _forwardModel->getKernel();
            for(int i = start; i < stop; i++)
            {
                l_i = i * _receivers.count * _sources.count;
                for(int j = 0; j < _receivers.count; j++)
                {
                    l_j = j * _sources.count;
                    for(int k = 0; k < _sources.count; k++)
                    {
                        kDummy = kappa[l_i + l_j + k];
                        kDummy.conjugate();
                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
            for(int i = 1; i < world.size(); i++)
            {
                std::vector<std::complex<double>> receiv;
                world.recv(i, 5, receiv);
                kRes += receiv;
            }
        }

        void ConjugateGradientMPIInversion::getUpdateDirectionInformationMPI(
            std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes, const int offset, const int block_size)
        {
            mpi::communicator world;
            mpi::request req = world.irecv(0, offset, res);
            req.wait();

            int start, stop;
            determineStartStopMPI(start, stop, _frequencies.count);

            int l_i, l_j;
            kRes.zero();
            core::dataGrid2D<std::complex<double>> kDummy(_grid);
            auto kappa = _forwardModel->getKernel();
            for(int i = start; i < stop; i++)
            {
                l_i = i * _receivers.count * _sources.count;
                for(int j = 0; j < _receivers.count; j++)
                {
                    l_j = j * _sources.count;
                    for(int k = 0; k < _sources.count; k++)
                    {
                        kDummy = kappa[l_i + l_j + k];
                        kDummy.conjugate();
                        kRes += kDummy * res[l_i + l_j + k];
                    }
                }
            }
            world.send(0, block_size, kRes.getData());
        }

    }   // namespace inversionMethods
}   // namespace fwi
#endif
