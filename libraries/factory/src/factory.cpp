#include "FiniteDifferenceForwardModel.h"
#include "FiniteDifferenceForwardModelInputCardReader.h"
#include "FiniteDifferenceForwardModelParallel.h"
#ifdef MPI
#include "FiniteDifferenceForwardModelParallelMPI.h"
#include "conjugateGradientMPIInversion.h"
#endif
#include "FixedStepSizeCalculator.h"
#include "GradientDescentDirectionCalculator.h"
#include "conjugateGradientInversion.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "evolutionInversion.h"
#include "evolutionInversionInputCardReader.h"
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
        : _costCalculator(core::CostFunctionCalculator::CostFunctionEnum::leastSquares)
        , _createdInversion()
        , _createdForwardModel()
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
#ifdef MPI
        if(desiredInversion == "ConjugateGradientMPIInversion")
        {
            inversionMethods::ConjugateGradientInversionInputCardReader conjugateGradientReader(gInput.caseFolder);
            _createdInversion = new inversionMethods::ConjugateGradientMPIInversion(costCalculator, forwardModel, conjugateGradientReader.getInput());
            return _createdInversion;
        }
#endif
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

        if(desiredForwardModel == "FiniteDifferenceParallelForwardModel")
        {
            forwardModels::finiteDifferenceForwardModelInputCardReader finitedifferencereader(caseFolder);
            _createdForwardModel =
                new forwardModels::FiniteDifferenceForwardModelParallel(grid, sources, receivers, frequencies, finitedifferencereader.getInput());
            return _createdForwardModel;
        }
#ifdef MPI

        if(desiredForwardModel == "FiniteDifferenceParallelMPIForwardModel")
        {
            forwardModels::finiteDifferenceForwardModelInputCardReader finitedifferencereader(caseFolder);
            _createdForwardModel =
                new forwardModels::FiniteDifferenceForwardModelMPI(grid, sources, receivers, frequencies, finitedifferencereader.getInput());
            return _createdForwardModel;
        }
#endif
        L_(io::linfo) << "The ForwardModel " << desiredForwardModel << " was not found";
        throw std::invalid_argument("The ForwardModel " + desiredForwardModel + " was not found");
    }

    void Factory::checkForwardModelExistence(forwardModels::ForwardModelInterface *forwardModel)
    {
        if(forwardModel == nullptr)
        {
            throw std::invalid_argument("ForwardModel does not exist");
        }
    }
}   // namespace fwi
