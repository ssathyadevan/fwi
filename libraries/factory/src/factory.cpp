#include "conjugateGradientInversion.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "evolutionInversion.h"
#include "evolutionInversionInputCardReader.h"
#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "gradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "log.h"
#include "randomInversion.h"
#include "randomInversionInputCardReader.h"
#include <factory.h>
#include <iostream>

inversionInterface *Factory::createInversion(std::string desired_inversion, forwardModelInterface *forwardModel, const genericInput gInput)
{
    inversionInterface *inversion;
    if(desired_inversion == "conjugateGradientInversion")
    {
        ConjugateGradientInversionInputCardReader conjugategradientreader(gInput.caseFolder);
        inversion = new ConjugateGradientInversion(forwardModel, conjugategradientreader.getInput());
        return inversion;
    }

    if(desired_inversion == "randomInversion")
    {
        RandomInversionInputCardReader randomreader(gInput.caseFolder);
        inversion = new RandomInversion(forwardModel, randomreader.getInput());
        return inversion;
    }

    if(desired_inversion == "gradientDescentInversion")
    {
        gradientDescentInversionInputCardReader gradientdescentreader(gInput.caseFolder);
        inversion = new gradientDescentInversion(forwardModel, gradientdescentreader.getInput());
        return inversion;
    }

    if(desired_inversion == "evolutionInversion")
    {
        EvolutionInversionInputCardReader evolutionreader(gInput.caseFolder);
        inversion = new EvolutionInversion(forwardModel, evolutionreader.getInput());
        return inversion;
    }
    L_(linfo) << "The Inversion method " << desired_inversion << " was not found";
    exit(EXIT_FAILURE);
}

forwardModelInterface *Factory::createForwardModel(const genericInput &gInput, const std::string desired_forward_model, const grid2D &grid, const sources &src,
    const receivers &recv, const frequenciesGroup &freq)
{
    forwardModelInterface *model;
    if(desired_forward_model == "integralForwardModel")
    {
        integralForwardModelInputCardReader integralreader(gInput.caseFolder);
        model = new IntegralForwardModel(grid, src, recv, freq, integralreader.getInput());
        return model;
    }
    if(desired_forward_model == "finiteDifferenceForwardModel")
    {
        finiteDifferenceForwardModelInputCardReader finitedifferencereader(gInput.caseFolder);
        model = new finiteDifferenceForwardModel(grid, src, recv, freq, finitedifferencereader.getInput());
        return model;
    }
    L_(linfo) << "The ForwardModel " << desired_forward_model << " was not found";
    exit(EXIT_FAILURE);
}
