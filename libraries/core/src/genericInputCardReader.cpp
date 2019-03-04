#include "genericInputCardReader.h"
#include <iostream>
#include "json.h"

genericInputCardReader::genericInputCardReader(std::string inputCardPath, std::string outputLocation, std::string runName): inputCardReader()
{
    readCard(inputCardPath);
    _input.outputLocation = outputLocation;
    _input.runName = runName;
}

genericInput genericInputCardReader::getInput()
{
    return _input;
}

void genericInputCardReader::readCard(std::string inputCardPath){
    std::ifstream in(inputCardPath + "GenericInput.json");
    nlohmann::json j;
    in >> j;

    double fMax = j["Freq"]["max"];
    double fMin = j["Freq"]["min"];
    int nFreq = j["Freq"]["nTotal"];
    const double spacing = (fMax - fMin)/(nFreq-1);

    genericInput jsonInput
    {
        inputCardPath, "", "",
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
