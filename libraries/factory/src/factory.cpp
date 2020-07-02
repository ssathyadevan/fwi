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

namespace fwi
{
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

    inversionMethods::inversionInterface *Factory::createInversion(
        const std::string &desiredInversion, forwardModels::forwardModelInterface *forwardModel, const io::genericInput &gInput)
    {
        if(desiredInversion == "conjugateGradientInversion")
        {
            inversionMethods::ConjugateGradientInversionInputCardReader conjugateGradientReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::ConjugateGradientInversion(forwardModel, conjugateGradientReader.getInput());
            return _createdInversion;
        }

        if(desiredInversion == "randomInversion")
        {
            inversionMethods::RandomInversionInputCardReader randomReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::RandomInversion(forwardModel, randomReader.getInput());
            return _createdInversion;
        }

        if(desiredInversion == "gradientDescentInversion")
        {
            inversionMethods::gradientDescentInversionInputCardReader gradientDescentReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::gradientDescentInversion(forwardModel, gradientDescentReader.getInput());
            return _createdInversion;
        }

        if(desiredInversion == "evolutionInversion")
        {
            inversionMethods::EvolutionInversionInputCardReader evolutionReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::EvolutionInversion(forwardModel, evolutionReader.getInput());
            return _createdInversion;
        }
        L_(io::linfo) << "The Inversion method " << desiredInversion << " was not found";
        throw std::invalid_argument("The Inversion method " + desiredInversion + " was not found");
    }

    forwardModels::forwardModelInterface *Factory::createForwardModel(const std::string &caseFolder, const std::string &desiredForwardModel,
        const core::grid2D &grid, const core::Sources &sources, const core::Receivers &receivers, const core::FrequenciesGroup &frequencies)
    {
        if(desiredForwardModel == "integralForwardModel")
        {
            forwardModels::integralForwardModelInputCardReader integralreader(caseFolder);
            _createdForwardModel = new forwardModels::IntegralForwardModel(grid, sources, receivers, frequencies, integralreader.getInput());
            return _createdForwardModel;
        }
        if(desiredForwardModel == "finiteDifferenceForwardModel")
        {
            forwardModels::finiteDifferenceForwardModelInputCardReader finitedifferencereader(caseFolder);
            _createdForwardModel = new forwardModels::finiteDifferenceForwardModel(grid, sources, receivers, frequencies, finitedifferencereader.getInput());
            return _createdForwardModel;
        }
        L_(io::linfo) << "The ForwardModel " << desiredForwardModel << " was not found";
        throw std::invalid_argument("The ForwardModel " + desiredForwardModel + " was not found");
    }

    void Factory::createStepSizeCalculator(
        const inversionMethods::StepSizeParameters &stepSizeParameters, const std::string &desiredStepSizeMethod, const core::grid2D &grid)
    {
        if(desiredStepSizeMethod == "FixedStepSize")
        {
            _createdStepSizeCalculator = new inversionMethods::FixedStepSizeCalculator(stepSizeParameters.initialStepSize);
            return;
        }
        if(desiredStepSizeMethod == "BorzilaiBorweinStepSize")
        {
            _createdStepSizeCalculator = new inversionMethods::BorzilaiBorweinStepSizeCalculator(grid, stepSizeParameters.initialStepSize);
            return;
        }
        if(desiredStepSizeMethod == "ConjugateGradientStepSize")
        {
            _createdStepSizeCalculator = new inversionMethods::ConjugateGradientStepSizeCalculator(grid, stepSizeParameters.initialStepSize);
            return;
        }

        L_(io::linfo) << "The Step size method " << desiredStepSizeMethod << " was not found";
        throw std::invalid_argument("The Step size method " + desiredStepSizeMethod + " was not found");
    }

    bool Factory::splittableInversion(const std::string inversionMethod)
    {
        bool splittable = true;
        if(inversionMethod == "ConjugateGradientRegularisationStepSize")   
        {
            splittable = false;
        }

        return splittable;
    }

    void Factory::createDirectionCalculator(const inversionMethods::DirectionParameters &directionParameters, const std::string &desiredDirectionMethod,
        forwardModels::forwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData)
    {
        const double errorFunctionalScalingFactor = 1.0 / (forwardModels::normSq(pData, pData.size()));

        if(desiredDirectionMethod == "ConjugateGradientDirection")
        {
            _createdDirectionCalculator = new inversionMethods::ConjugateGradientDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
            return;
        }
        if(desiredDirectionMethod == "GradientDescentDirection")
        {
            _createdDirectionCalculator = new inversionMethods::GradientDescentDirectionCalculator(
                errorFunctionalScalingFactor, forwardModel, directionParameters.derivativeStepSize, pData);
            return;
        }
        L_(io::linfo) << "The Direction method " << desiredDirectionMethod << " was not found";
        throw std::invalid_argument("The Direction method " + desiredDirectionMethod + " was not found");
    }

    void Factory::createCombinedDirectionAndStepSize(forwardModels::forwardModelInterface *forwardModel,
        const inversionMethods::StepSizeParameters &stepSizeParameters, const inversionMethods::ReconstructorParameters &reconstructorParameters,
        const std::vector<std::complex<double>> &pData, const std::string &desiredCombinedDirectionAndStepSizeMethod)
    {   // desiredCombinedDirectionAndStepSizeMethod is actually desiredStepSizeMethod
        if(desiredCombinedDirectionAndStepSizeMethod == "ConjugateGradientRegularisationStepSize")
        {
            inversionMethods::ConjugateGradientWithRegularisationParametersInput cgParametersInput;
            cgParametersInput._tolerance = reconstructorParameters.tolerance;
            cgParametersInput._nIterations = reconstructorParameters.maxIterationsNumber;
            cgParametersInput._deltaAmplification._slope = stepSizeParameters.slope;
            cgParametersInput._deltaAmplification._start = stepSizeParameters.initialStepSize;

            const double errorFunctionalScalingFactor = 1.0 / (forwardModels::normSq(pData, pData.size()));
            inversionMethods::ConjugateGradientWithRegularisationCalculator *OneInstance =
                new inversionMethods::ConjugateGradientWithRegularisationCalculator(errorFunctionalScalingFactor, forwardModel, cgParametersInput, pData);
            _createdStepSizeCalculator = OneInstance;
            _createdDirectionCalculator = OneInstance;
            return;
        }

        L_(io::linfo) << "The combined Direction and StepSize method " << desiredCombinedDirectionAndStepSizeMethod << " was not found";
        throw std::invalid_argument("The combined Direction and StepSize method " + desiredCombinedDirectionAndStepSizeMethod + " was not found");
    }

    inversionMethods::StepAndDirectionReconstructor *Factory::createStepAndDirectionReconstructor(
        const inversionMethods::StepAndDirectionReconstructorInput &stepAndDirectionInput, forwardModels::forwardModelInterface *forwardModel,
        const std::string &desiredStepSizeMethod, const std::string &desiredDirectionMethod, const std::vector<std::complex<double>> &pData)
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

        _createdReconstructor = new inversionMethods::StepAndDirectionReconstructor(
            _createdStepSizeCalculator, _createdDirectionCalculator, forwardModel, stepAndDirectionInput.reconstructorParameters);

        return _createdReconstructor;
    }

    void Factory::checkForwardModelExistence(forwardModels::forwardModelInterface *forwardModel)
    {
        if(forwardModel == nullptr)
        {
            throw std::invalid_argument("ForwardModel does not exist");
        }
    }
}   // namespace fwi
