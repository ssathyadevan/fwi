#pragma once

#include "ForwardModelInterface.h"
#include "genericInput.h"
#include "inversionInterface.h"
#include <string>
#include "CostFunctionCalculator.h"

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

    private:
        const core::CostFunctionCalculator _costCalculator;
        inversionMethods::inversionInterface *_createdInversion;
        forwardModels::ForwardModelInterface *_createdForwardModel;

        /**
         * @brief checkForwardModelExistence, makes sure the forwardmodel is created
         */
        void checkForwardModelExistence(forwardModels::ForwardModelInterface *forwardModel);

    };
}   // namespace fwi
