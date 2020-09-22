#include "BorzilaiBorweinStepSizeCalculator.h"
#include "ConjugateGradientDirectionCalculator.h"
#include "ConjugateGradientStepSizeCalculator.h"
#include "ConjugateGradientWithRegularisationCalculator.h"
#include "DummyDirectionCalculator.h"
#include "DummyStepSizeCalculator.h"
#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "FixedStepSizeCalculator.h"
#include "GradientDescentDirectionCalculator.h"
#include "conjugateGradientInversion.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "evolutionInversion.h"
#include "evolutionInversionInputCardReader.h"
#include "gradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#ifdef	specialDependencyFound
	#include "newPropietaryInversion.h"
#endif
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
        : _costCalculator(core::CostFunctionCalculator::CostFunctionEnum::leastSquares)
        , _createdInversion()
        , _createdForwardModel()
        , _createdStepSizeCalculator()
        , _createdDirectionCalculator()
        , _createdReconstructor()
        , _splittable(true)
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
            _createdForwardModel = nullptr;
        }

        if(_createdStepSizeCalculator != nullptr)
        {
            delete _createdStepSizeCalculator;
            _createdStepSizeCalculator = nullptr;
        }

		// If the _createdStepSizeCalculator is so that _splittable == false, we can only delete the element pointed by _createdDirectionCalculator and _createdStepSizeCalculator once. 
		// Once ConjugateGradientWithRegularisation is refactored and validated (and thus inherits uniquely from _StepSizeCalculator) the _splittable parameter and all of its occurrences can be removed, and the following if() can be fixed.
        if(_createdDirectionCalculator != nullptr && _splittable)
        {
            delete _createdDirectionCalculator;
        }
        _createdDirectionCalculator = nullptr;

        if(_createdReconstructor != nullptr)
        {
            delete _createdReconstructor;
        }
    }
    const core::CostFunctionCalculator costCalculator(core::CostFunctionCalculator::CostFunctionEnum::leastSquares);
    inversionMethods::inversionInterface *Factory::createInversion(
        const std::string &desiredInversion, forwardModels::ForwardModelInterface *forwardModel, const io::genericInput &gInput)
    {
        if(desiredInversion == "ConjugateGradientInversion")
        {
            inversionMethods::ConjugateGradientInversionInputCardReader conjugateGradientReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::ConjugateGradientInversion(costCalculator, forwardModel, conjugateGradientReader.getInput());
            return _createdInversion;
        }

        if(desiredInversion == "RandomInversion")
        {
            inversionMethods::RandomInversionInputCardReader randomReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::RandomInversion(costCalculator, forwardModel, randomReader.getInput());
            return _createdInversion;
        }

        if(desiredInversion == "GradientDescentInversion")
        {
            inversionMethods::gradientDescentInversionInputCardReader gradientDescentReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::gradientDescentInversion(costCalculator, forwardModel, gradientDescentReader.getInput());
            return _createdInversion;
        }

        if(desiredInversion == "EvolutionInversion")
        {
            inversionMethods::EvolutionInversionInputCardReader evolutionReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::EvolutionInversion(costCalculator, forwardModel, evolutionReader.getInput());
            return _createdInversion;
        }
		#ifdef	specialDependencyFound
		if(desiredInversion == "NewPropietaryInversion")
        {
            inversionMethods::EvolutionInversionInputCardReader evolutionReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::NewPropietaryInversion();
            return _createdInversion;
        }
		#endif
        L_(io::linfo) << "The Inversion method " << desiredInversion << " was not found";
        throw std::invalid_argument("The Inversion method " + desiredInversion + " was not found");
    }

    forwardModels::ForwardModelInterface *Factory::createForwardModel(const std::string &caseFolder, const std::string &desiredForwardModel,
        const core::grid2D &grid, const core::Sources &sources, const core::Receivers &receivers, const core::FrequenciesGroup &frequencies)
    {
        if(desiredForwardModel == "IntegralForwardModel")
        {
            forwardModels::integralForwardModelInputCardReader integralreader(caseFolder);
            _createdForwardModel = new forwardModels::IntegralForwardModel(grid, sources, receivers, frequencies, integralreader.getInput());
            return _createdForwardModel;
        }
        if(desiredForwardModel == "FiniteDifferenceForwardModel")
        {
            forwardModels::finiteDifferenceForwardModelInputCardReader finitedifferencereader(caseFolder);
            _createdForwardModel = new forwardModels::FiniteDifferenceForwardModel(grid, sources, receivers, frequencies, finitedifferencereader.getInput());
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
        if(desiredStepSizeMethod == "DummyStepSize")   // DummyStepSize, update with your own.
        {
            _createdStepSizeCalculator = new inversionMethods::DummyStepSizeCalculator();
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
        forwardModels::ForwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData)
    {
        const double errorFunctionalScalingFactor = 1.0 / (core::l2NormSquared(pData));

        if(desiredDirectionMethod == "ConjugateGradientDirection")
        {
            _createdDirectionCalculator = new inversionMethods::ConjugateGradientDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
            return;
        }
        if(desiredDirectionMethod == "GradientDescentDirection")
        {
            _createdDirectionCalculator = new inversionMethods::GradientDescentDirectionCalculator(
                errorFunctionalScalingFactor, costCalculator, forwardModel, directionParameters.derivativeStepSize, pData);
            return;
        }
        if(desiredDirectionMethod == "DummyStepSize")   // DummyDirection, update with your own.
        {
            _createdDirectionCalculator = new inversionMethods::DummyDirectionCalculator(errorFunctionalScalingFactor, forwardModel);
            return;
        }
        L_(io::linfo) << "The Direction method " << desiredDirectionMethod << " was not found";
        throw std::invalid_argument("The Direction method " + desiredDirectionMethod + " was not found");
    }

    void Factory::createCombinedDirectionAndStepSize(forwardModels::ForwardModelInterface *forwardModel,
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

            const double errorFunctionalScalingFactor = 1.0 / (core::l2NormSquared(pData));
            inversionMethods::ConjugateGradientWithRegularisationCalculator *OneInstance = new inversionMethods::ConjugateGradientWithRegularisationCalculator(
                errorFunctionalScalingFactor, costCalculator, forwardModel, cgParametersInput, pData);
            _createdStepSizeCalculator = OneInstance;
            _createdDirectionCalculator = OneInstance;
            return;
        }

        L_(io::linfo) << "The combined Direction and StepSize method " << desiredCombinedDirectionAndStepSizeMethod << " was not found";
        throw std::invalid_argument("The combined Direction and StepSize method " + desiredCombinedDirectionAndStepSizeMethod + " was not found");
    }

    inversionMethods::StepAndDirectionReconstructor *Factory::createStepAndDirectionReconstructor(
        const inversionMethods::StepAndDirectionReconstructorInput &stepAndDirectionInput, forwardModels::ForwardModelInterface *forwardModel,
        const std::string &desiredStepSizeMethod, const std::string &desiredDirectionMethod, const std::vector<std::complex<double>> &pData)
    {
        checkForwardModelExistence(forwardModel);
        _splittable = splittableInversion(desiredStepSizeMethod);

        if(_splittable)
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
            _createdStepSizeCalculator, _createdDirectionCalculator, costCalculator, forwardModel, stepAndDirectionInput.reconstructorParameters);

        return _createdReconstructor;
    }

    void Factory::checkForwardModelExistence(forwardModels::ForwardModelInterface *forwardModel)
    {
        if(forwardModel == nullptr)
        {
            throw std::invalid_argument("ForwardModel does not exist");
        }
    }
}   // namespace fwi
