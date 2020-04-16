#include "ConjugateGradientDirectionCalculator.h"
#include "FixedStepSizeCalculator.h"
#include "GradientDescentDirectionCalculator.h"
#include "conjugateGradientInversion.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "evolutionInversion.h"
#include "evolutionInversionInputCardReader.h"
#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "gradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"
#include "randomInversion.h"
#include "randomInversionInputCardReader.h"
#include <factory.h>
#include <iostream>

inversionInterface *Factory::createInversion(std::string desiredInversion, forwardModelInterface *forwardModel, const genericInput gInput)
{
    inversionInterface *inversion;
    if(desiredInversion == "conjugateGradientInversion")
    {
        ConjugateGradientInversionInputCardReader conjugategradientreader(gInput.caseFolder);
        inversion = new ConjugateGradientInversion(forwardModel, conjugategradientreader.getInput());
        return inversion;
    }

    if(desiredInversion == "randomInversion")
    {
        RandomInversionInputCardReader randomreader(gInput.caseFolder);
        inversion = new RandomInversion(forwardModel, randomreader.getInput());
        return inversion;
    }

    if(desiredInversion == "gradientDescentInversion")
    {
        gradientDescentInversionInputCardReader gradientdescentreader(gInput.caseFolder);
        inversion = new gradientDescentInversion(forwardModel, gradientdescentreader.getInput());
        return inversion;
    }

    if(desiredInversion == "evolutionInversion")
    {
        EvolutionInversionInputCardReader evolutionreader(gInput.caseFolder);
        inversion = new EvolutionInversion(forwardModel, evolutionreader.getInput());
        return inversion;
    }
    L_(linfo) << "The Inversion method " << desiredInversion << " was not found";
    exit(EXIT_FAILURE);
}

forwardModelInterface *Factory::createForwardModel(const std::string caseFolder, const std::string desiredForwardModel, const grid2D &grid,
    const sources &sources, const receivers &recveivers, const frequenciesGroup &frequencies)
{
    forwardModelInterface *model;
    if(desiredForwardModel == "integralForwardModel")
    {
        integralForwardModelInputCardReader integralreader(caseFolder);
        model = new IntegralForwardModel(grid, sources, recveivers, frequencies, integralreader.getInput());
        return model;
    }
    if(desiredForwardModel == "finiteDifferenceForwardModel")
    {
        finiteDifferenceForwardModelInputCardReader finitedifferencereader(caseFolder);
        model = new finiteDifferenceForwardModel(grid, sources, recveivers, frequencies, finitedifferencereader.getInput());
        return model;
    }
    L_(linfo) << "The ForwardModel " << desiredForwardModel << " was not found";
    exit(EXIT_FAILURE);
}

StepSizeCalculator *Factory::createStepSizeCalculator(const std::string caseFolder, const std::string desiredStepSizeMethod)
{
    StepSizeCalculator *stepSizeCalculator;

    // If statement to chose the method
    if(desiredStepSizeMethod == "fixedStepSize")
    {
        // Read and/or compute input
        (void)caseFolder;
        double stepSize = 1.0;

        // Create step size calculator
        stepSizeCalculator = new FixedStepSizeCalculator(stepSize);
        return stepSizeCalculator;
    }
    L_(linfo) << "The Step size method " << desiredStepSizeMethod << " was not found";
    throw std::invalid_argument("The Step size method " + desiredStepSizeMethod + " was not found");
}

DirectionCalculator *Factory::createDirectionCalculator(
    const std::string caseFolder, const std::string desiredDirectionMethod, forwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData)
{
    DirectionCalculator *directionCalculator;
    // Compute error functional scaling factor
    const double errorFunctionalScalingFactor = 1.0 / (normSq(pData, pData.size()));

    // If statement to chose the method
    if(desiredDirectionMethod == "conjugateGradientDirection")
    {
        // Read and/or compute input
        (void)caseFolder;

        // Create direction calculator
        directionCalculator = new ConjugateGradientDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
        return directionCalculator;
    }
    if(desiredDirectionMethod == "gradientDescentDirection")
    {
        // Read and/or compute input
        (void)caseFolder;
        const double derivativeStepSize = 0.1;

        // Create direction calculator
        directionCalculator = new GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardModel, derivativeStepSize, pData);
        return directionCalculator;
    }
    L_(linfo) << "The Direction method " << desiredDirectionMethod << " was not found";
    throw std::invalid_argument("The Direction method " + desiredDirectionMethod + " was not found");
}
