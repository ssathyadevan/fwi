#include "BorzilaiBorweinStepSizeCalculator.h"
#include "ConjugateGradientDirectionCalculator.h"
#include "ConjugateGradientStepSizeCalculator.h"
#include "ConjugateGradientWithRegularisationCalculator.h"
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

Factory::Factory()
    : _createdInversion()
    , _createdForwardModel()
    , _createdStepSizeCalculator()
    , _createdDirectionCalculator()
    , _createdReconstructor()
{
}

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

inversionInterface *Factory::createInversion(const std::string &desiredInversion, forwardModelInterface *forwardModel, const io::genericInput &gInput)
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
    L_(io::linfo) << "The Inversion method " << desiredInversion << " was not found";
    throw std::invalid_argument("The Inversion method " + desiredInversion + " was not found");
}

forwardModelInterface *Factory::createForwardModel(const std::string &caseFolder, const std::string &desiredForwardModel, const core::grid2D &grid,
    const core::sources &sources, const core::receivers &receivers, const core::frequenciesGroup &frequencies)
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
    L_(io::linfo) << "The ForwardModel " << desiredForwardModel << " was not found";
    throw std::invalid_argument("The ForwardModel " + desiredForwardModel + " was not found");
}

void Factory::createStepSizeCalculator(const StepSizeParameters &stepSizeParameters, const std::string &desiredStepSizeMethod, const core::grid2D &grid)
{
    if(desiredStepSizeMethod == "fixedStepSize")
    {
        _createdStepSizeCalculator = new FixedStepSizeCalculator(stepSizeParameters.initialStepSize);
        return;
    }
    if(desiredStepSizeMethod == "BorzilaiBorwein")
    {
        _createdStepSizeCalculator = new BorzilaiBorweinStepSizeCalculator(grid, stepSizeParameters.initialStepSize);
        return;
    }
    if(desiredStepSizeMethod == "ConjugateGradient")
    {
        _createdStepSizeCalculator = new ConjugateGradientStepSizeCalculator(grid, stepSizeParameters.initialStepSize);
        return;
    }

    L_(io::linfo) << "The Step size method " << desiredStepSizeMethod << " was not found";
    throw std::invalid_argument("The Step size method " + desiredStepSizeMethod + " was not found");
}

bool Factory::splittableInversion(const std::string inversionMethod)
{
    bool splittable = true;
    if(inversionMethod == "conjugateGradientRegularisationStepSize")   // decide name
    {
        splittable = false;
    }

    return splittable;
}

void Factory::createDirectionCalculator(const DirectionParameters &directionParameters, const std::string &desiredDirectionMethod,
    forwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData)
{
    const double errorFunctionalScalingFactor = 1.0 / (normSq(pData, pData.size()));

    if(desiredDirectionMethod == "conjugateGradientDirection")
    {
        _createdDirectionCalculator = new ConjugateGradientDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
        return;
    }
    if(desiredDirectionMethod == "gradientDescentDirection")
    {
        _createdDirectionCalculator =
            new GradientDescentDirectionCalculator(errorFunctionalScalingFactor, forwardModel, directionParameters.derivativeStepSize, pData);
        return;
    }
    L_(io::linfo) << "The Direction method " << desiredDirectionMethod << " was not found";
    throw std::invalid_argument("The Direction method " + desiredDirectionMethod + " was not found");
}

void Factory::createCombinedDirectionAndStepSize(forwardModelInterface *forwardModel, const StepSizeParameters &stepSizeParameters,
    const ReconstructorParameters &reconstructorParameters, const std::vector<std::complex<double>> &pData,
    const std::string &desiredCombinedDirectionAndStepSizeMethod)
{   // desiredCombinedDirectionAndStepSizeMethod is actually desiredStepSizeMethod
    if(desiredCombinedDirectionAndStepSizeMethod == "conjugateGradientRegularisation")
    {
        ConjugateGradientWithRegularisationParametersInput cgParametersInput;
        cgParametersInput._tolerance = reconstructorParameters.tolerance;
        cgParametersInput._nIterations = reconstructorParameters.maxIterationsNumber;
        cgParametersInput._deltaAmplification._slope = stepSizeParameters.slope;
        cgParametersInput._deltaAmplification._start = stepSizeParameters.initialStepSize;

        const double errorFunctionalScalingFactor = 1.0 / (normSq(pData, pData.size()));
        ConjugateGradientWithRegularisationCalculator *OneInstance =
            new ConjugateGradientWithRegularisationCalculator(errorFunctionalScalingFactor, forwardModel, cgParametersInput, pData);
        _createdStepSizeCalculator = OneInstance;
        _createdDirectionCalculator = OneInstance;
        return;
    }

    L_(io::linfo) << "The combined Direction and StepSize method " << desiredCombinedDirectionAndStepSizeMethod << " was not found";
    throw std::invalid_argument("The combined Direction and StepSize method " + desiredCombinedDirectionAndStepSizeMethod + " was not found");
}

StepAndDirectionReconstructor *Factory::createStepAndDirectionReconstructor(const StepAndDirectionReconstructorInput &stepAndDirectionInput,
    forwardModelInterface *forwardModel, const std::string &desiredStepSizeMethod, const std::string &desiredDirectionMethod,
    const std::vector<std::complex<double>> &pData)
{
    checkForwardModelExistence(forwardModel);

    if(splittableInversion(desiredStepSizeMethod))
    {
        L_(io::linfo) << "Create StepSizeCalculator...";
        createStepSizeCalculator(stepAndDirectionInput.stepSizeParameters, desiredStepSizeMethod, forwardModel->getGrid());

        L_(io::linfo) << "Create DirectionCalculator...";
        createDirectionCalculator(stepAndDirectionInput.directionParameters, desiredDirectionMethod, forwardModel, pData);
    }

    else
    {
        createCombinedDirectionAndStepSize(
            forwardModel, stepAndDirectionInput.stepSizeParameters, stepAndDirectionInput.reconstructorParameters, pData, desiredStepSizeMethod);
    }

    _createdReconstructor =
        new StepAndDirectionReconstructor(_createdStepSizeCalculator, _createdDirectionCalculator, forwardModel, stepAndDirectionInput.reconstructorParameters);

    return _createdReconstructor;
}

void Factory::checkForwardModelExistence(forwardModelInterface *forwardModel)
{
    if(forwardModel == nullptr)
    {
        throw std::invalid_argument("ForwardModel does not exist");
    }
}
