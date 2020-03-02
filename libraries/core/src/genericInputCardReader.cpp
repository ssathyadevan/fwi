#include "genericInputCardReader.h"
#include <iostream>
#include <exception>
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
    checkinput();
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

void GenericInputCardReader::checkinput()
{
    if (_input.c_0 <= 0 ){throw std::invalid_argument("Invalid value for c_0 in GenericInput.json.");}
    if (_input.freq.min > _input.freq.max ) {throw std::invalid_argument("Invalid ranges for frequenties in GenericInput.json.");}
    if (_input.freq.nTotal <= 0 ) {throw std::invalid_argument("Invalid number of frequenties in GenericInput.json.");}
    if (_input.reservoirTopLeftCornerInM[0] > _input.reservoirBottomRightCornerInM[0] || _input.reservoirTopLeftCornerInM[1] > _input.reservoirBottomRightCornerInM[1]) {throw std::invalid_argument("Invalid ranges for reservoir in GenericInput.json.");}
    if (_input.receiversTopLeftCornerInM[0] > _input.receiversBottomRightCornerInM[0] || _input.receiversTopLeftCornerInM[1] > _input.receiversBottomRightCornerInM[1]) {throw std::invalid_argument("Invald ranges for reseivers in GenericInput.json.");}
    if (_input.sourcesTopLeftCornerInM[0] > _input.sourcesBottomRightCornerInM[0] || _input.sourcesTopLeftCornerInM[1] > _input.sourcesBottomRightCornerInM[1]) {throw std::invalid_argument("Invalid ranges for reservoir in GenericInput.json.");}
    if (_input.ngrid_original[0] <= 0 || _input.ngrid_original[1] <= 0) {throw std::invalid_argument("Invalid grid_original input in GenericInput.json.");}
    if (_input.ngrid[0] <= 0 || _input.ngrid[1] <= 0) {throw std::invalid_argument("Invalid grid input in GenericInput.json.");}
    if (_input.nSourcesReceivers.nreceivers <= 0) {throw std::invalid_argument("Invalid number of receivers in GenericInput.json.");}
    if (_input.nSourcesReceivers.nsources <= 0) {throw std::invalid_argument("Invalid number of sources in GenericInput.json.");}
}
