#pragma once

#include "genericInput.h"
#include "inversionInterface.h"
#include "forwardModelInterface.h"
#include <string>


class Factory
{

public:
    Factory() {}

    static InversionInterface* createInversion(std::string desired_inversion, ForwardModelInterface *forwardModel, const GenericInput gInput);
    static ForwardModelInterface* createForwardModel(const GenericInput &gInput, const std::string desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
                    const FrequenciesGroup &freq);
};
