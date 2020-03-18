#include <factory.h>
#include "conjugateGradientInversion.h"
#include "conjugateGradientInversionInputCardReader.h"
#include "randomInversion.h"
#include "randomInversionInputCardReader.h"
#include "evolutionInversion.h"
#include "evolutionInversionInputCardReader.h"
#include "gradientDescentInversion.h"
#include "gradientDescentInversionInputCardReader.h"
#include "integralForwardModel.h"
#include "integralForwardModelInputCardReader.h"
#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include "log.h"
#include "OpenMPgradientDescentInversion.h"

#include <iostream>

InversionInterface *Factory::createInversion(std::string desired_inversion, ForwardModelInterface *forwardModel, const GenericInput gInput)
{
        InversionInterface *inversion;
        if (desired_inversion == "conjugateGradientInversion")
        {
                ConjugateGradientInversionInputCardReader conjugategradientreader(gInput.caseFolder);
                inversion = new ConjugateGradientInversion(forwardModel, conjugategradientreader.getInput());
                return inversion;
        }

        if (desired_inversion == "randomInversion")
        {
                RandomInversionInputCardReader randomreader(gInput.caseFolder);
                inversion = new RandomInversion(forwardModel, randomreader.getInput());
                return inversion;
        }

        if (desired_inversion == "gradientDescentInversion")
        {
                GradientDescentInversionInputCardReader gradientdescentreader(gInput.caseFolder);
                inversion = new GradientDescentInversion(forwardModel, gradientdescentreader.getInput());
                return inversion;
        }
        if (desired_inversion == "evolutionInversion")
        {
                EvolutionInversionInputCardReader evolutionreader(gInput.caseFolder);
                inversion = new EvolutionInversion(forwardModel, evolutionreader.getInput());
                return inversion;
        }
        /*if (desired_inversion == "OpenmpgradientDescentInversion")
        {
                inversion = new OpenMPGradientDescentInversion(forwardModel, gInput);
                return inversion;
        }*/
        L_(linfo) << "The Inversion method "<< desired_inversion <<" was not found" ;
        exit(EXIT_FAILURE);
}

ForwardModelInterface* Factory::createForwardModel(const GenericInput &gInput, const std::string desired_forward_model, const Grid2D &grid, const Sources &src, const Receivers &recv,
                    const FrequenciesGroup &freq){
       
    ForwardModelInterface *model;
    if (desired_forward_model == "integralForwardModel"){
        IntegralForwardModelInputCardReader integralreader(gInput.caseFolder);
        model = new IntegralForwardModel(grid, src, recv, freq, integralreader.getInput());
        return model;
    }
    if (desired_forward_model == "finiteDifferenceForwardModel"){
        FiniteDifferenceForwardModelInputCardReader finitedifferencereader(gInput.caseFolder);
        model = new FiniteDifferenceForwardModel(grid, src, recv, freq, finitedifferencereader.getInput());
        return model;
    }
    L_(linfo) << "The ForwardModel "<< desired_forward_model <<" was not found" ;
    exit(EXIT_FAILURE);

}
