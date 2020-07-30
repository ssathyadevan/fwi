#pragma once

#include "ConjugateGradientWithRegularisationParametersInput.h"
#include "CostFunctionCalculator.h"
#include "DirectionCalculator.h"
#include "ForwardModelInterface.h"
#include "StepAndDirectionReconstructor.h"
#include "StepAndDirectionReconstructorInput.h"
#include "StepSizeCalculator.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>

namespace fwi
{
    class Factory
    {
    public:
        /**
         * @brief Factory, is the owner of models, reconstructors and calculators.
         */
        Factory();
        ~Factory();

        inversionMethods::inversionInterface *createInversion(
            const std::string &desiredInversion, forwardModels::ForwardModelInterface *forwardModel, const io::genericInput &gInput);
        forwardModels::ForwardModelInterface *createForwardModel(const std::string &caseFolder, const std::string &desiredForwardModel,
            const core::grid2D &grid, const core::Sources &sources, const core::Receivers &receivers, const core::FrequenciesGroup &frequencies);

        /**
         * @brief createStepAndDirectionReconstructor, creates stepSizeCalculator and directionCalculator
         * ToDo: Get input from files, idea: read files outside the factory and give them as input for the factory.
         * @param caseFolder is a string
         * @param forwardmodel is a pointer to ForwardModelInterface
         * @param desiredStepSizeMethod is a string
         * @param desiredDirectionMethod is a string
         * @param pData is a vector of complex doubles, represents the measurement for each combination of sources, receivers and frequencies
         * @return
         */
        inversionMethods::StepAndDirectionReconstructor *createStepAndDirectionReconstructor(
            const inversionMethods::StepAndDirectionReconstructorInput &stepAndDirectionInput, forwardModels::ForwardModelInterface *forwardmodel,
            const std::string &desiredStepSizeMethod, const std::string &desiredDirectionMethod, const std::vector<std::complex<double>> &pData);

    private:
        inversionMethods::inversionInterface *_createdInversion;
        forwardModels::ForwardModelInterface *_createdForwardModel;
        inversionMethods::StepSizeCalculator *_createdStepSizeCalculator;
        inversionMethods::DirectionCalculator *_createdDirectionCalculator;
        inversionMethods::StepAndDirectionReconstructor *_createdReconstructor;

        /**
         * @brief checkForwardModelExistence, makes sure the forwardmodel is created
         */
        void checkForwardModelExistence(forwardModels::ForwardModelInterface *forwardModel);

        /**
         * @brief createStepSizeCalculator, reads in parameters from file and creates the desired step size calculator.
         * @param caseFolder is a string, the path to input files
         * @param desiredStepSizeMethod is a string
         * @return StepSizecallculator
         */
        void createStepSizeCalculator(
            const inversionMethods::StepSizeParameters &stepSizeParameters, const std::string &desiredStepSizeMethod, const core::grid2D &grid);

        /**
         * @brief splittableInversion, checks whether we can have two different objects for the inversionMethods::DirectionCalculator and
         * inversionMethods::StepSizeCalculator or not
         * @param the name of the StepSize method (between StepSizeCalculators and DirectionCalculators, the former are more delicate)
         * @return a boolean containing the possibility to have split objects
         */

        bool splittableInversion(const std::string inversionMethod);
        /**
         * @brief createDirectionCalculator, reads in parameter from file and creates the desired direction calculator
         * @param caseFolder is a string, the path to input files
         * @param desiredDirectionMethod is a string
         * @param forwardModel
         * @param pData is a vector of complex doubles, represents the measurement for each combination of sources, receivers and frequencies
         * @return DirectionCalculator
         */
        void createDirectionCalculator(const inversionMethods::DirectionParameters &directionParameters, const std::string &desiredDirectionMethod,
            forwardModels::ForwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData);
        /**
         * @brief createCombinedDirectionAndStepSize, in case we cannot split our inversionMethods::StepSizeCalculator and DirectionCalculator, here we create
         * only one object and we point both related pointers of inversionMethods::StepAndDirectionReconstructor to it.
         * @param forwardModel
         * @param stepSizeParameters, a struct containing some parameters used to tweak the descent algorithm
         * @param reconstructorParameters, a struct containing some parameters related to the desired performances for the descent algorithm
         * @param pData, the data we want to simulate
         * @param desiredCombinedDirectionAndStepSizeMethod, the actual descent algorithm we want to implement
         */
        void createCombinedDirectionAndStepSize(forwardModels::ForwardModelInterface *forwardModel,
            const inversionMethods::StepSizeParameters &stepSizeParameters, const inversionMethods::ReconstructorParameters &reconstructorParameters,
            const std::vector<std::complex<double>> &pData, const std::string &desiredCombinedDirectionAndStepSizeMethod);
    };
}   // namespace fwi
