#ifdef MPI
#pragma once
#include "CostFunctionCalculator.h"
#include "ForwardModelInterface.h"
#include "conjugateGradientInversion.h"
#include "conjugateGradientInversionInput.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include "log.h"
#include "regularization.h"
#include <fstream>
#include <sstream>

namespace fwi
{
    namespace inversionMethods
    {
        /**
         * @brief The ConjugateGradientInversion class uses the conjugate gradient method, as described in the README and the thesis by Peter Haffinger,
         * to iteratively estimate the underlying structure using pressure data measured by the core::Receivers and a forward model, which is able to convert
         * an estimated structure to simulated pressure data.
         */

        class ConjugateGradientMPIInversion : public ConjugateGradientInversion
        {
        public:
            ConjugateGradientMPIInversion(const core::CostFunctionCalculator &costCalculator, forwardModels::ForwardModelInterface *forwardModel,
                const ConjugateGradientInversionInput &invInput);

            ~ConjugateGradientMPIInversion();

            // Overloading specifc functions for MPI
            void getUpdateDirectionInformation(
                const std::vector<std::complex<double>> &residualVector, core::dataGrid2D<std::complex<double>> &kappaTimesResidual);

            // MPI specific functions
            void getUpdateDirectionInformationMPI(
                std::vector<std::complex<double>> &res, core::dataGrid2D<std::complex<double>> &kRes, const int offset, const int block_size);
            void determineStartStopMPI(int &start, int &stop, const int &freq);
        };
    }   // namespace inversionMethods
}   // namespace fwi
#endif
