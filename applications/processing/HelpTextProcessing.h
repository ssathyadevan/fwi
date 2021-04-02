#pragma once
#include <string>

const std::string VERSION_PROCESSING = "FWI_PreProcessing.exe - Version 0.1 - 15/03/2021";

const std::string HELP_TEXT_PROCESSING = 
    VERSION_PROCESSING + " - Help\n" +
    "Usage: FWI_PreProcessing.exe [-h | -v | [option] ]\n" +
    "All options:\n" +
    "-h, --help               : Prints this help message.\n" +
    "-v, --version            : Prints the current version.\n" +
    "-d, --dir <d_arg>        : Provides the path to the case folder as <d_arg>.\n" +
    "          -d ./                          : Default option, current working directory.\n" +
    "          -d Some/Path/                  : Pathname without spaces don't need quotes\n"+
    "          -d \"Some/Path/With Spaces/\"    : Pathname with spaces need quotes\n"+
    "-f, --forward <f_arg>    : Sets the forward model of the processing step to <f_arg>. Available optionas are:\n" +
    "          -f FiniteDifference            : Default option\n" +
    "          -f Integral                         \n" +
    "-i, --inversion <i_arg>  : Sets the inversion model of the processing step to <i_arg>. Available options are:\n" +
    "          -i ConjugateGradient           : Default option\n" +
    "          -i GradientDescent                             \n" +
    "          -i Evolution                                   \n" +
    "          -i Random                                      \n" +
    "          -i StepAndDirection            : Depricated    \n" +
    "    If StepAndDirection is chosen, the values for --step-dir and --step-size must be provided.\n" +
    "--step-dir <arg1>        : Sets the stepdirection method in the StepAndDirection processing to <arg1>. Options are:\n" +
    "          --step-dir ConjugateGradient    \n" +
    "          --step-dir GradientDescent      \n" +
    "--step-size <arg2>       : Sets the stepsize method in the StepAndDirection processing to <arg2>. Options are:\n" +
    "          --step-size ConjugateGradient               : works best with --step-dir ConjugateGradient\n" + 
    "          --step-size ConjugateGradientRegularization : Works best with --step-dir ConjugateGradient\n" + 
    "          --step-size BorzilaiBorwein                 : Works best with --step-dir GradientDescent  \n" + 
    "          --step-size Fixed\n" +
 
    "All options are optional. Options are immediately followed by their arguments if required. All options can be used in any order,\n"+ 
    "except -h and -v, which are required to be first. Any unrecognized options are ignored (silently).\n\n" +

    "Examples:\n" + 
    "FWI_Processing.exe -h\n" + 
    "FWI_Processing.exe -v\n" +
    "FWI_Processing.exe -d /path/to/folder\n" + 
    "FWI_Processing.exe -d \"/path/to/folder/with spaces/\" -f Integral\n"
    "FWI_Processing.exe -d /path/to/folder -f Integral -i EvolutionInversion\n" +
    "FWI_Processing.exe -d /path/to/folder -f Integral -i StepAndDirection --step-dir ConjugateGradient --step-size ConjugateGradientRegularization\n"
;