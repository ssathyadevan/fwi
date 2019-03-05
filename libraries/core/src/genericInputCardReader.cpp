#include "genericInputCardReader.h"
#include <iostream>
#include "json.h"

genericInputCardReader::genericInputCardReader(const std::string &pathToCardSet, const std::string &outputLocation) : inputCardReader()
{
    // pathToCardSet will generally look something like this: ../input/default/
    // we split this into inputfolder = "../input/" and runName = "default"
    std::string temp;

    //remove trailing slash
    unsigned int lastCharIndex = pathToCardSet.size()-1;
    if(pathToCardSet[lastCharIndex] == '/')
    {
        temp = pathToCardSet.substr(0,lastCharIndex);
    } else {
        temp = pathToCardSet;
    }

    unsigned int idx = temp.find_last_of('/');
    std::string inputFolder, runName;

    if (idx != std::string::npos)
    {
        inputFolder = temp.substr(0, idx+1);
        runName = temp.substr(idx+1);
    }

    _input.inputFolder = inputFolder;
    _input.runName = runName;

    readCard(pathToCardSet);

    //add slash to outputLocation if it didn't end with one
    if(outputLocation[outputLocation.size()-1] != '/')
    {
        _input.outputLocation = outputLocation + '/';
    } else {
        _input.outputLocation = outputLocation;
    }
}

genericInput genericInputCardReader::getInput()
{
    return _input;
}

void genericInputCardReader::readCard(std::string inputCardPath)
{
    std::string filePath = _input.inputFolder + _input.runName + "/GenericInput.json";
    std::ifstream in(filePath);

    if(!in.is_open())
    {
        std::cout << "Could not open file at " << filePath << std::endl;
        std::exit(EXIT_FAILURE);
    }

    nlohmann::json j;
    in >> j;

    double fMax = j["Freq"]["max"];
    double fMin = j["Freq"]["min"];
    int nFreq = j["Freq"]["nTotal"];
    const double spacing = (fMax - fMin)/(nFreq-1);

    genericInput jsonInput
    {
        _input.inputFolder, "", _input.runName,
        j["c_0"],
        {j["Freq"]["min"], j["Freq"]["max"], j["Freq"]["nTotal"], spacing},
        {j["reservoirTopLeft"]["x"], j["reservoirTopLeft"]["z"]},
        {j["reservoirBottomRight"]["x"], j["reservoirBottomRight"]["z"]},
        {j["sourcesTopLeft"]["x"], j["sourcesTopLeft"]["z"]},
        {j["sourcesBottomRight"]["x"], j["sourcesBottomRight"]["z"]},
        {j["receiversTopLeft"]["x"], j["receiversTopLeft"]["z"]},
        {j["receiversBottomRight"]["x"], j["receiversBottomRight"]["z"]},
        {j["ngrid"]["x"], j["ngrid"]["z"]},
        {j["nSources"], j["nReceivers"]},
        j["fileName"],
        j["verbosity"]
    };

    _input = jsonInput;
}
