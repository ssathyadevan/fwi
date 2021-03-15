#include <iostream>
#include <vector>
#include "log.h"
#include "genericInputCardReader.h"
#include "FullFWIOptions.h"



// Unused?
const std::vector<std::string> POSSIBLE_ARGS = {
    "-d", "--dir"
    "-p", "--preprocess",
    "-s", "--solver",
    "-i", "--inversion",
    "--stepdir",
    "--stepsize",
    "--skip-post",
    "--skip-pre"
};

const std::string HELP_TEXT = "This is the help message.";

const std::string VERSION = "Version 0.1 - 15/03/2021";

std::vector<std::string> readArguments(int argc, char* argv[]);
void printHelpOrVersion(const std::string& firstArg);

int main(int argc, char* argv[])
{
    try
    {
        std::vector<std::string> arguments = readArguments(argc, argv);
        printHelpOrVersion(arguments[0]);
        FullFWIOptions fwiOpts(arguments);

        // Dummy code: Print all options as read from the input arguments
        std::cout << "dir=" << fwiOpts.dir << std::endl;
        std::cout << "preprocess=" << fwiOpts.preprocess << std::endl;
        std::cout << "solver=" << fwiOpts.solver << std::endl;
        std::cout << "inversion=" << fwiOpts.inversion << std::endl;
        std::cout << "stepdir=" << fwiOpts.stepdir << std::endl;
        std::cout << "stepsize=" << fwiOpts.stepsize << std::endl;
        std::cout << "skippost=" << fwiOpts.skippost << std::endl;
        std::cout << "skippre=" << fwiOpts.skippre << std::endl;

        /* Code to later setup logging
        fwi::io::genericInputCardReader genericReader(arguments[0]);
        const fwi::io::genericInput gInput = genericReader.getInput();

        std::string logFilePath = gInput.outputLocation + gInput.runName + "PreProcess.log";
        */
            
    }
    catch(const std::exception& e)
    {
        std::cerr << e.what() << std::endl;
        std::exit(EXIT_FAILURE);
    }
}



std::vector<std::string> readArguments(int argc, char* argv[])
{
    // First argument is always the executable location, we can ignore that.
    if (argc == 1)
        throw std::invalid_argument(
            "No arguments received. Program requires -d or --dir flag specifying the path to the input and output folders. \nUse -h for further help."
        );
    std::vector<std::string> arguments(argv + 1, argv + argc);
    
    // Append empty entry to avoid looking for flag option value beyond set arguments.
    arguments.push_back("");
    return arguments;
}



void printHelpOrVersion(const std::string& firstArg)
{
    if(firstArg == "-h" or firstArg == "--help")
    {
        std::cout << HELP_TEXT << std::endl;
        std::exit(EXIT_SUCCESS);
    }

    if(firstArg == "-v" or firstArg == "--version")
    {
        std::cout << VERSION << std::endl;
        std::exit(EXIT_SUCCESS);
    }
}