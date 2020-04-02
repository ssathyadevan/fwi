#pragma once

#include "genericInput.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include <string>


class Factory
{

public:
    Factory() {}

    static inversionInterface * createInversion(std::string desired_inversion, forwardModelInterface *forwardModel, const genericInput gInput);
    static forwardModelInterface * createForwardModel(const genericInput &gInput, const std::string desired_forward_model, const grid2D &grid, const sources &src, const receivers &recv,
                    const frequenciesGroup &freq);
};
