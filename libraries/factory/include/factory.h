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
    Factory() {}

    static inversionInterface *createInversion(std::string desiredInversion, forwardModelInterface *forwardModel, const genericInput gInput);
    static forwardModelInterface *createForwardModel(const std::string caseFolder, const std::string desiredForwardModel, const grid2D &grid,
        const sources &sources, const receivers &recveivers, const frequenciesGroup &frequencies);
    /**
     * @brief createStepSizeCalculator, reads in parameters from file and creates the desired step size calculator.
     * @param caseFolder is a string, the path to input files
     * @param desiredStepSizeMethod is a string
     * @return StepSizecallculator
     */
    static StepSizeCalculator *createStepSizeCalculator(const std::string caseFolder, const std::string desiredStepSizeMethod);

    /**
     * @brief createDirectionCalculator, reads in parameter from file and creates the desired direction calculator
     * @param caseFolder is a string, the path to input files
     * @param desiredDirectionMethod is a string
     * @param forwardModel
     * @param pData is a vector of complex doubles, represents the measurement for each combination of sources, receivers and frequencies
     * @return DirectionCalculator
     */
    static DirectionCalculator *createDirectionCalculator(const std::string caseFolder, const std::string desiredDirectionMethod,
        forwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData);
};
