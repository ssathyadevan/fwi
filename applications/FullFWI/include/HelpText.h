#pragma once
#include <string>

const std::string VERSION = "Version 0.1 - 15/03/2021";

const std::string HELP_TEXT = 
    "\n\nFullFWI.exe - " + VERSION + " - Help\n" +
    "Usage: FullFWI.exe [-h | -v | -d <d_arg> [option] ]\n" +
    "All options:\n" +
    "-h, --help               : Prints this help message.\n" +
    "-v, --version            : Prints the current version.\n" +
    "-d, --dir <d_arg>        : Required. Provides the path to the case folder as <d_arg>.\n" +
    "-p, --preprocess <p_arg> : Sets the forward model of the preprocessing step to <p_arg>. Available options are:\n\n" +

    "        -p FiniteDifference : Default option\n" +
    "        -p Integral                         \n\n" +

    "-f, --forward <f_arg>    : Sets the forward model of the processing step to <f_arg>. Available optionas are:\n\n" +

    "        -f FiniteDifference : Default option\n" +
    "        -f Integral                         \n\n" +

    "-i, --inversion <i_arg>  : Sets the inversion model of the processing step to <i_arg>. Available options are:\n\n" +

    "         -i StepAndDirection           : Default option\n" +
    "         -i ConjugateGradientInversion                 \n" +
    "         -i RandomInversion                            \n" +
    "         -i GradientDescentInversion                   \n" +
    "         -i EvolutionInversion                         \n\n" +

    "       If StepAndDirection is chosen, the values for --step-dir and --step-size are used.\n" +
    "       Otherwise, the values for --step-dir and --step-size are ignored (silently).\n\n" +

    "--step-dir <arg1>        : Sets the stepdirection method in the StepAndDirection processing to <arg1>. Options are:\n\n" +

    "          --step-dir ConjugateGradient  : Default option\n" +
    "          --step-dir GradientDescent                    \n\n" +

    "--step-size <arg2>       : Sets the stepsize method in the StepAndDirection processing to <arg2>. Options are:\n\n" +

    "          --step-size ConjugateGradient               : Default option, works best with --step-dir ConjugateGradient\n" + 
    "          --step-size ConjugateGradientRegularization : Works best with --step-dir ConjugateGradient\n" + 
    "          --step-size BorzilaiBorwein                 : Works best with --step-dir GradientDescent  \n" + 
    "          --step-size Fixed                           \n\n" +

    "--skip-pre               : Skip the preprocessing step. Ignores -p <p_arg>.\n" +
    "--skip-process           : Skip the data processing step. Ignores -f, -i, --step-dir, --step-size.\n" +
    "--skip-post              : Skip the postprocessing step.\n\n" + 

    "Options are immediately followed by their arguments if required. All options can be used in any order, except -h and -v,\n" +
    "which are required to be first. -d is the only required option, all others have default values if not specified.\n\n" +

    "Examples:\n" + 
    "FullFWI.exe -h\n" + 
    "FullFWI.exe -v\n" +
    "FullFWI.exe -d /path/to/folder\n" + 
    "FullFWI.exe -d /path/to/folder -p FiniteDifference -f Integral -i GradientDescentInversion\n" + 
    "FullFWI.exe -d /path/to/folder -p FiniteDifference -f Integral -i GradientDescentInversion\n" + 
    "FullFWI.exe -d /path/to/folder --skip-pre -f Integral -i GradientDescentInversion\n" + 
    "FullFWI.exe -d /path/to/folder -i StepAndDirection --step-dir GradientDescent --step-size BorzilaiBorwein --skip-post\n\n"
;