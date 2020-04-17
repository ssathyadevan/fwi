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
    static StepSizeCalculator *createStepSizeCalculator(const std::string caseFolder, const std::string desiredStepSizeMethod);
    static DirectionCalculator *createDirectionCalculator(const std::string caseFolder, const std::string desiredDirectionMethod,
        forwardModelInterface *forwardModel, const std::vector<std::complex<double>> &pData);
};
