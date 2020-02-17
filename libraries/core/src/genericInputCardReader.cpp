#include "genericInputCardReader.h"
#include <iostream>
#include "json.h"

GenericInputCardReader::GenericInputCardReader(const std::string &caseFolder_)
{
    std::string caseFolder = caseFolder_;
    if (caseFolder[caseFolder.size() - 1] == '/')
    {
        caseFolder = caseFolder.substr(0, caseFolder.size() - 1);
    }

    std::string runName = caseFolder;

    const std::size_t idx = runName.find_last_of('/');
    if (std::string::npos != idx)
    {
        runName = runName.substr(idx + 1);
    }
    if (runName == ".")
        runName = "default";

    _input.caseFolder = caseFolder;
    _input.inputFolder = caseFolder + "/input/";
    _input.runName = runName;
    _input.outputLocation = caseFolder + "/output/";

    readCard(caseFolder);
}

GenericInput GenericInputCardReader::getInput()
{
    return _input;
}

void GenericInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/GenericInput.json");

    nlohmann::json::json_pointer p_ngridx("/ngrid/x");
    nlohmann::json::json_pointer p_ngridz("/ngrid/z");
    nlohmann::json::json_pointer p_ngrid_original_x("/ngrid_original/x");
    nlohmann::json::json_pointer p_ngrid_original_z("/ngrid_original/z");

    int n_gid_default_x = j.value(p_ngridx, 0);
    int n_gid_default_z = j.value(p_ngridz, 0);

    GenericInput jsonInput{
        _input.caseFolder, _input.inputFolder, _input.outputLocation, _input.runName, j["c_0"], {j["Freq"]["min"], j["Freq"]["max"], j["Freq"]["nTotal"]}, {j["reservoirTopLeft"]["x"], j["reservoirTopLeft"]["z"]}, {j["reservoirBottomRight"]["x"], j["reservoirBottomRight"]["z"]}, {j["sourcesTopLeft"]["x"], j["sourcesTopLeft"]["z"]}, {j["sourcesBottomRight"]["x"], j["sourcesBottomRight"]["z"]}, {j["receiversTopLeft"]["x"], j["receiversTopLeft"]["z"]}, {j["receiversBottomRight"]["x"], j["receiversBottomRight"]["z"]}, {j.value(p_ngrid_original_x, n_gid_default_x), j.value(p_ngrid_original_z, n_gid_default_z)}, {j["ngrid"]["x"], j["ngrid"]["z"]}, {j["nSources"], j["nReceivers"]}, j["fileName"], j["verbosity"]};

    _input = jsonInput;
}
