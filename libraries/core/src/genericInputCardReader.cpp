#include "genericInputCardReader.h"
#include <iostream>
#include <exception>
#include "json.h"

genericInputCardReader::genericInputCardReader(const std::string &caseFolder_)
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
    checkInput();
}

genericInput genericInputCardReader::getInput()
{
    return _input;
}

void genericInputCardReader::readCard(const std::string &caseFolder)
{
    nlohmann::json j = readFile(caseFolder + "/input/genericInput.json");

    nlohmann::json::json_pointer p_ngridx("/nGrid/x");
    nlohmann::json::json_pointer p_ngridz("/nGrid/z");
    nlohmann::json::json_pointer p_ngrid_original_x("/nGridOriginal/x");
    nlohmann::json::json_pointer p_ngrid_original_z("/nGridOriginal/z");

    int n_gid_default_x = j.value(p_ngridx, 0);
    int n_gid_default_z = j.value(p_ngridz, 0);

    genericInput jsonInput{
        _input.caseFolder, _input.inputFolder, _input.outputLocation, _input.runName, j["c0"], {j["Freq"]["min"], j["Freq"]["max"], j["Freq"]["nTotal"]}, {j["reservoirTopLeft"]["x"], j["reservoirTopLeft"]["z"]}, {j["reservoirBottomRight"]["x"], j["reservoirBottomRight"]["z"]}, {j["sourcesTopLeft"]["x"], j["sourcesTopLeft"]["z"]}, {j["sourcesBottomRight"]["x"], j["sourcesBottomRight"]["z"]}, {j["receiversTopLeft"]["x"], j["receiversTopLeft"]["z"]}, {j["receiversBottomRight"]["x"], j["receiversBottomRight"]["z"]}, {j.value(p_ngrid_original_x, n_gid_default_x), j.value(p_ngrid_original_z, n_gid_default_z)}, {j["nGrid"]["x"], j["nGrid"]["z"]}, {j["nSources"], j["nReceivers"]}, j["fileName"], j["verbosity"]};

    _input = jsonInput;
}

void genericInputCardReader::checkInput()
{
    if (_input.c0 <= 0 ){throw std::invalid_argument("Invalid value for c0 in genericInput.json.");}
    if (_input.freq.min >= _input.freq.max || _input.freq.min <= 0) {throw std::invalid_argument("Invalid ranges for frequenties in genericInput.json.");}
    if (_input.freq.nTotal <= 1 ) {throw std::invalid_argument("Invalid number of frequenties in genericInput.json.");}
    if (_input.reservoirTopLeftCornerInM[0] > _input.reservoirBottomRightCornerInM[0] || _input.reservoirTopLeftCornerInM[1] > _input.reservoirBottomRightCornerInM[1]) {throw std::invalid_argument("Invalid ranges for reservoir in genericInput.json.");}
    if (_input.receiversTopLeftCornerInM[0] > _input.receiversBottomRightCornerInM[0] || _input.receiversTopLeftCornerInM[1] > _input.receiversBottomRightCornerInM[1]) {throw std::invalid_argument("Invald ranges for reseivers in genericInput.json.");}
    if (_input.sourcesTopLeftCornerInM[0] > _input.sourcesBottomRightCornerInM[0] || _input.sourcesTopLeftCornerInM[1] > _input.sourcesBottomRightCornerInM[1]) {throw std::invalid_argument("Invalid ranges for reservoir in genericInput.json.");}
    if (_input.nGridOriginal[0] <= 0 || _input.nGridOriginal[1] <= 0) {throw std::invalid_argument("Invalid grid_original input in genericInput.json.");}
    if (_input.nGrid[0] < 3 || _input.nGrid[1] < 3) {throw std::invalid_argument("Invalid grid input in genericInput.json.");}
    if (_input.nSourcesReceivers.nReceivers <= 1) {throw std::invalid_argument("Invalid number of receivers in genericInput.json.");}
    if (_input.nSourcesReceivers.nSources <= 1) {throw std::invalid_argument("Invalid number of sources in genericInput.json.");}
}
