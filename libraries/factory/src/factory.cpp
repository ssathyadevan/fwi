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

Factory::Factory() : _createdInversion(), _createdForwardModel(), _createdStepSizeCalculator(), _createdDirectionCalculator(), _createdReconstructor() {}

Factory::~Factory()
{
    if(_createdInversion != nullptr)
    {
        delete _createdInversion;
    }

    if(_createdForwardModel != nullptr)
    {
        delete _createdForwardModel;
    }

    if(_createdStepSizeCalculator != nullptr)
    {
        delete _createdStepSizeCalculator;
    }

    if(_createdDirectionCalculator != nullptr)
    {
        delete _createdDirectionCalculator;
    }
    if(_createdReconstructor != nullptr)
    {
        delete _createdReconstructor;
    }
}

inversionInterface *Factory::createInversion(const std::string &desiredInversion, forwardModelInterface *forwardModel, const genericInput &gInput)
{
    if(desiredInversion == "conjugateGradientInversion")
    {
        ConjugateGradientInversionInputCardReader conjugateGradientReader(gInput.caseFolder);
        _createdInversion = new ConjugateGradientInversion(forwardModel, conjugateGradientReader.getInput());
        return _createdInversion;
    }

    if(desiredInversion == "randomInversion")
    {
        RandomInversionInputCardReader randomReader(gInput.caseFolder);
        _createdInversion = new RandomInversion(forwardModel, randomReader.getInput());
        return _createdInversion;
    }

    if(desiredInversion == "gradientDescentInversion")
    {
        gradientDescentInversionInputCardReader gradientDescentReader(gInput.caseFolder);
        _createdInversion = new gradientDescentInversion(forwardModel, gradientDescentReader.getInput());
        return _createdInversion;
    }

    if(desiredInversion == "evolutionInversion")
    {
        EvolutionInversionInputCardReader evolutionReader(gInput.caseFolder);
        _createdInversion = new EvolutionInversion(forwardModel, evolutionReader.getInput());
        return _createdInversion;
    }
    L_(linfo) << "The Inversion method " << desiredInversion << " was not found";
    throw std::invalid_argument("The Inversion method " + desiredInversion + " was not found");
}

forwardModelInterface *Factory::createForwardModel(const std::string &caseFolder, const std::string &desiredForwardModel, const grid2D &grid,
    const sources &sources, const receivers &receivers, const frequenciesGroup &frequencies)
{
    if(desiredForwardModel == "integralForwardModel")
    {
        integralForwardModelInputCardReader integralreader(caseFolder);
        _createdForwardModel = new IntegralForwardModel(grid, sources, receivers, frequencies, integralreader.getInput());
        return _createdForwardModel;
    }
    if(desiredForwardModel == "finiteDifferenceForwardModel")
    {
        finiteDifferenceForwardModelInputCardReader finitedifferencereader(caseFolder);
        _createdForwardModel = new finiteDifferenceForwardModel(grid, sources, receivers, frequencies, finitedifferencereader.getInput());
        return _createdForwardModel;
    }
    L_(linfo) << "The ForwardModel " << desiredForwardModel << " was not found";
    throw std::invalid_argument("The ForwardModel " + desiredForwardModel + " was not found");
}

void Factory::createStepSizeCalculator(const std::string &caseFolder, const std::string &desiredStepSizeMethod)
{
    if(desiredStepSizeMethod == "fixedStepSize")
    {
        // Read and/or compute input
        (void)caseFolder;
        const double stepSize = 1.0;

        // Create step size calculator
        _createdStepSizeCalculator = new FixedStepSizeCalculator(stepSize);
        return;
    }
    L_(linfo) << "The Step size method " << desiredStepSizeMethod << " was not found";
    throw std::invalid_argument("The Step size method " + desiredStepSizeMethod + " was not found");
}

void Factory::createDirectionCalculator(const std::string &caseFolder, const std::string &desiredDirectionMethod, forwardModelInterface *forwardModel,
    const std::vector<std::complex<double>> &pData)
{
    const double errorFunctionalScalingFactor = 1.0 / (normSq(pData, pData.size()));

    if(desiredDirectionMethod == "conjugateGradientDirection")
    {
        // Read and/or compute input
        (void)caseFolder;

        // Create direction calculator
        _createdDirectionCalculator = new ConjugateGradientDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
        return;
    }
    if(desiredDirectionMethod == "gradientDescentDirection")
    {
        // Read and/or compute input
        (void)caseFolder;
        const double derivativeStepSize = 0.1;

        // Create direction calculator
        _createdDirectionCalculator = new GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardModel, derivativeStepSize, pData);
        return;
    }
    L_(linfo) << "The Direction method " << desiredDirectionMethod << " was not found";
    throw std::invalid_argument("The Direction method " + desiredDirectionMethod + " was not found");
}

StepAndDirectionReconstructor *Factory::createStepAndDirectionReconstructor(const std::string &caseFolder, forwardModelInterface *forwardModel,
    const std::string &desiredStepSizeMethod, const std::string &desiredDirectionMethod, const std::vector<std::complex<double>> &pData)
{
    checkForwardModelExistence(forwardModel);

    L_(linfo) << "Create StepSizeCalculator...";
    createStepSizeCalculator(caseFolder, desiredStepSizeMethod);

    L_(linfo) << "Create DirectionCalculator...";
    createDirectionCalculator(caseFolder, desiredDirectionMethod, forwardModel, pData);

    // read input from file
    (void)caseFolder;
    double tolerance = 0.001;
    dataGrid2D startingChi(forwardModel->getGrid());
    int maxIterationNumber = 4;
    double h = 1.0;
    DirectionInput directionInput = {tolerance, startingChi, maxIterationNumber, h};

    _createdReconstructor = new StepAndDirectionReconstructor(_createdStepSizeCalculator, _createdDirectionCalculator, forwardModel, directionInput);

    return _createdReconstructor;
}

void Factory::checkForwardModelExistence(forwardModelInterface *forwardModel)
{
    if(forwardModel == nullptr)
    {
        throw std::invalid_argument("ForwardModel does not exist");
    }
}
