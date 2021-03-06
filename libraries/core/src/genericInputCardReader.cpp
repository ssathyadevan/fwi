#include "genericInputCardReader.h"
#include <iostream>
#include "json.h"

genericInputCardReader::genericInputCardReader(const std::string &caseFolder_) : inputCardReader()
{
    std::string caseFolder = caseFolder_;
    if (caseFolder[caseFolder.size()-1] == '/')
    {
       caseFolder = caseFolder.substr(0, caseFolder.size()-1);
    }

    std::string runName = caseFolder;
    const unsigned int idx = runName.find_last_of('/');
    if (std::string::npos != idx)
    {
        runName = runName.substr(idx + 1);
    }

    _input.caseFolder = caseFolder;
    _input.inputFolder = caseFolder + "/input/";
    _input.runName = runName;
    _input.outputLocation = caseFolder + "/output/";

    readCard(caseFolder);
}

genericInput genericInputCardReader::getInput()
{
    return _input;
}

void genericInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/GenericInput.json");

    double fMax = j["Freq"]["max"];
    double fMin = j["Freq"]["min"];
    int nFreq = j["Freq"]["nTotal"];
    const double spacing = (fMax - fMin)/(nFreq-1);

    genericInput jsonInput
    {
        _input.caseFolder, _input.inputFolder, _input.outputLocation, _input.runName,
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
