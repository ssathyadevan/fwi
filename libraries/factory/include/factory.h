#pragma once

#include "DirectionCalculator.h"
#include "StepSizeCalculator.h"
#include "forwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>

class Factory
{
public:
    Factory();
    ~Factory();

    inversionInterface *createInversion(const std::string &desiredInversion, forwardModelInterface *forwardModel, const genericInput &gInput);
    forwardModelInterface *createForwardModel(const std::string &caseFolder, const std::string &desiredForwardModel, const grid2D &grid, const sources &sources,
        const receivers &receivers, const frequenciesGroup &frequencies);
    /**
     * @brief createStepSizeCalculator, reads in parameters from file and creates the desired step size calculator.
     * @param caseFolder is a string, the path to input files
     * @param desiredStepSizeMethod is a string
     * @return StepSizecallculator
     */
    StepSizeCalculator *createStepSizeCalculator(const std::string &caseFolder, const std::string &desiredStepSizeMethod);

    /**
     * @brief createDirectionCalculator, reads in parameter from file and creates the desired direction calculator
     * @param caseFolder is a string, the path to input files
     * @param desiredDirectionMethod is a string
     * @param forwardModel
     * @param pData is a vector of complex doubles, represents the measurement for each combination of sources, receivers and frequencies
     * @return DirectionCalculator
     */
    DirectionCalculator *createDirectionCalculator(const std::string &caseFolder, const std::string &desiredDirectionMethod,
        forwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData);

private:
    inversionInterface *_createdInversion;
    forwardModelInterface *_createdForwardModel;
    StepSizeCalculator *_createdStepSizeCalculator;
    DirectionCalculator *_createdDirectionCalculator;
};
