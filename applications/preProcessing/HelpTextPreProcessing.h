#pragma once
#include <string>

const std::string VERSION_PREPROCESSING = "FWI_PreProcessing.exe - Version 0.1 - 15/03/2021";

const std::string HELP_TEXT_PREPROCESSING = 
    VERSION_PREPROCESSING + " - Help\n" +
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
 
    "All options are optional. Options are immediately followed by their arguments if required. All options can be used in any order,\n"+ 
    "except -h and -v, which are required to be first. Any unrecognized options are ignored (silently).\n\n" +

    "Examples:\n" + 
    "FWI_PreProcessing.exe -h\n" + 
    "FWI_PreProcessing.exe -v\n" +
    "FWI_PreProcessing.exe -d /path/to/folder\n" + 
    "FWI_PreProcessing.exe -d \"/path/to/folder/with spaces/\" -f Integral\n"
;