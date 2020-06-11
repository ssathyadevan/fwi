#pragma once

#include "ConjugateGradientWithRegularisationParametersInput.h"
#include "DirectionCalculator.h"
#include "StepAndDirectionReconstructor.h"
#include "StepAndDirectionReconstructorInput.h"
#include "StepSizeCalculator.h"
#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>

class Factory
{
public:
    /**
     * @brief Factory, is the owner of models, reconstructors and calculators.
     */
    Factory();
    ~Factory();

    inversionInterface *createInversion(const std::string &desiredInversion, forwardModelInterface *forwardModel, const genericInput &gInput);
    forwardModelInterface *createForwardModel(const std::string &caseFolder, const std::string &desiredForwardModel, const grid2D &grid, const sources &sources,
        const receivers &receivers, const frequenciesGroup &frequencies);

    /**
     * @brief createStepAndDirectionReconstructor, creates stepSizeCalculator and directionCalculator
     * ToDo: Get input from files, idea: read files outside the factory and give them as input for the factory.
     * @param caseFolder is a string
     * @param forwardmodel is a pointer to forwardmodelInterface
     * @param desiredStepSizeMethod is a string
     * @param desiredDirectionMethod is a string
     * @param pData is a vector of complex doubles, represents the measurement for each combination of sources, receivers and frequencies
     * @return
     */
    StepAndDirectionReconstructor *createStepAndDirectionReconstructor(const StepAndDirectionReconstructorInput &stepAndDirectionInput,
        forwardModelInterface *forwardmodel, const std::string &desiredStepSizeMethod, const std::string &desiredDirectionMethod,
        const std::vector<std::complex<double>> &pData);

private:
    inversionInterface *_createdInversion;
    forwardModelInterface *_createdForwardModel;
    StepSizeCalculator *_createdStepSizeCalculator;
    DirectionCalculator *_createdDirectionCalculator;
    StepAndDirectionReconstructor *_createdReconstructor;

    /**
     * @brief checkForwardModelExistence, makes sure the forwardmodel is created
     */
    void checkForwardModelExistence(forwardModelInterface *forwardModel);

    /**
     * @brief createStepSizeCalculator, reads in parameters from file and creates the desired step size calculator.
     * @param caseFolder is a string, the path to input files
     * @param desiredStepSizeMethod is a string
     * @return StepSizecallculator
     */
    void createStepSizeCalculator(const StepSizeParameters &stepSizeParameters, const std::string &desiredStepSizeMethod, const grid2D &grid);

    /**
     * @brief splittableInversion, checks whether we can have two different objects for the DirectionCalculator and StepSizeCalculator or not
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
    void createDirectionCalculator(const DirectionParameters &directionParameters, const std::string &desiredDirectionMethod,
        forwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData);
    /**
     * @brief createCombinedDirectionAndStepSize, in case we cannot split our StepSizeCalculator and DirectionCalculator, here we create only one object and we
     * point both related pointers of StepAndDirectionReconstructor to it.
     * @param forwardModel
     * @param stepSizeParameters, a struct containing some parameters used to tweak the descent algorithm
     * @param reconstructorParameters, a struct containing some parameters related to the desired performances for the descent algorithm
     * @param pData, the data we want to simulate
     * @param desiredCombinedDirectionAndStepSizeMethod, the actual descent algorithm we want to implement
     */
    void createCombinedDirectionAndStepSize(forwardModelInterface *forwardModel, const StepSizeParameters &stepSizeParameters,
        const ReconstructorParameters &reconstructorParameters, const std::vector<std::complex<double>> &pData,
        const std::string &desiredCombinedDirectionAndStepSizeMethod);
};
