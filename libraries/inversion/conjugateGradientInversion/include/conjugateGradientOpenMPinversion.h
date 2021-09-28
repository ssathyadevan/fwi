#ifndef CONJUGATEGRADIENTOPENMPINVERSION_H
#define CONJUGATEGRADIENTOPENMPINVERSION_H
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
        class conjugateGradientOpenMPinversion : public ConjugateGradientInversion
        {
        public:
            conjugateGradientOpenMPinversion(const core::CostFunctionCalculator &costCalculator, forwardModels::ForwardModelInterface *forwardModel,
                const ConjugateGradientInversionInput &invInput);

            // Overloading conjugateGradient parameters
            void getUpdateDirectionInformation(
                const std::vector<std::complex<double>> &residualVector, core::dataGrid2D<std::complex<double>> &kappaTimesResidual);
        };
    }   // namespace inversionMethods
}   // namespace fwi

#endif   // CONJUGATEGRADIENTOPENMPINVERSION_H
