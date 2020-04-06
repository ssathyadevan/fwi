#include "genericInputCardReader.h"
#include "json.h"
#include <exception>
#include <iostream>
#include <string.h>

genericInputCardReader::genericInputCardReader(const std::string &caseFolder_)
{
    std::string caseFolder = caseFolder_;
    removeLastSlash(caseFolder);
    readCard(caseFolder);
}

void genericInputCardReader::removeLastSlash(std::string &caseFolder)
{
    if(caseFolder[caseFolder.size() - 1] == '/')
    {
        caseFolder = caseFolder.substr(0, caseFolder.size() - 1);
    }
}

void genericInputCardReader::readCard(const std::string &caseFolder)
{
    std::string runName = getRunName(caseFolder);

    static const std::string stringJsonFilePath = "/input/GenericInput.json";
    nlohmann::json jsonFile = readFile(caseFolder + stringJsonFilePath);

    // Initialize genericInput
    genericInput jsonInput;
    jsonInput.caseFolder = caseFolder;
    jsonInput.runName = runName;
    static const std::string stringInputFolder = "/input/";
    jsonInput.inputFolder = caseFolder + stringInputFolder;
    static const std::string stringOutputFolder = "/output/";
    jsonInput.outputLocation = caseFolder + stringOutputFolder;

    // Read json input file
    static const std::string parameterFileName = "fileName";
    jsonInput.fileName = jsonFile[parameterFileName];

    readC0Parameter(jsonFile, jsonInput);
    readFreqParameter(jsonFile, jsonInput);
    readReservoirParameter(jsonFile, jsonInput);
    readSourcesParameter(jsonFile, jsonInput);
    readReceiversParameter(jsonFile, jsonInput);
    readGridOriginalParameter(jsonFile, jsonInput);
    readGridParameter(jsonFile, jsonInput);
    readNSourcesParameter(jsonFile, jsonInput);
    readNReceiversParameter(jsonFile, jsonInput);

    static const std::string parameterVerbose = "verbosity";
    jsonInput.verbose = jsonFile[parameterVerbose];

    _input = jsonInput;
    // TODO: return jsonInput reference and set in constructor for readability
}

std::string genericInputCardReader::getRunName(const std::string &caseFolder)
{
    std::string runName = caseFolder;

    // Remove directory part
    const std::size_t idx = runName.find_last_of('/');
    if(idx != std::string::npos)
    {
        runName = runName.substr(idx + 1);
    }

    if(runName == ".")
    {
        runName = "default";
    }

    return runName;
}

void genericInputCardReader::readC0Parameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterC0 = "c_0";
    double c0 = jsonFile[parameterC0];

    if(c0 <= 0)
    {
        throw std::invalid_argument("Invalid value for c0 in genericInput.json.");
    }

    jsonInput.c0 = c0;
}

void genericInputCardReader::readFreqParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterFreq = "Freq";
    static const std::string parameterFreqMin = "min";
    static const std::string parameterFreqMax = "max";
    static const std::string parameterFreqNTotal = "nTotal";

    double min = jsonFile[parameterFreq][parameterFreqMin];
    double max = jsonFile[parameterFreq][parameterFreqMax];
    if(min >= max || min <= 0)
    {
        throw std::invalid_argument("Invalid ranges for frequenties in genericInput.json.");
    }

    int nTotal = jsonFile[parameterFreq][parameterFreqNTotal];
    if(nTotal <= 1)
    {
        throw std::invalid_argument("Invalid number of frequenties in genericInput.json.");
    }
    jsonInput.freq = freqInfo(min, max, nTotal);
}

void genericInputCardReader::readReservoirParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterX = "x";
    static const std::string parameterZ = "z";

    static const std::string parameterReservoirTopLeft = "reservoirTopLeft";
    double reservoirTopLeftX = jsonFile[parameterReservoirTopLeft][parameterX];
    double reservoirTopLeftZ = jsonFile[parameterReservoirTopLeft][parameterZ];

    static const std::string parameterReservoirBottomRight = "reservoirBottomRight";
    double reservoirBottomRightX = jsonFile[parameterReservoirBottomRight][parameterX];
    double reservoirBottomRightZ = jsonFile[parameterReservoirBottomRight][parameterZ];

    if(reservoirTopLeftX > reservoirBottomRightX || reservoirTopLeftZ > reservoirBottomRightZ)
    {
        throw std::invalid_argument("Invalid ranges for reservoir in genericInput.json.");
    }

    jsonInput.reservoirBottomRightCornerInM = {reservoirBottomRightX, reservoirBottomRightZ};
    jsonInput.reservoirTopLeftCornerInM = {reservoirTopLeftX, reservoirTopLeftZ};
}

void genericInputCardReader::readSourcesParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterX = "x";
    static const std::string parameterZ = "z";

    static const std::string parameterSourcesTopLeft = "sourcesTopLeft";
    double sourcesTopLeftX = jsonFile[parameterSourcesTopLeft][parameterX];
    double sourcesTopLeftZ = jsonFile[parameterSourcesTopLeft][parameterZ];

    static const std::string parameterSourcesBottomRight = "sourcesBottomRight";
    double sourcesBottomRightX = jsonFile[parameterSourcesBottomRight][parameterX];
    double sourcesBottomRightZ = jsonFile[parameterSourcesBottomRight][parameterZ];

    if(sourcesTopLeftX > sourcesBottomRightX || sourcesTopLeftZ > sourcesBottomRightZ)
    {
        throw std::invalid_argument("Invalid ranges for sources in genericInput.json.");
    }

    jsonInput.sourcesTopLeftCornerInM = {sourcesTopLeftX, sourcesTopLeftZ};
    jsonInput.sourcesBottomRightCornerInM = {sourcesBottomRightX, sourcesBottomRightZ};
}

void genericInputCardReader::readReceiversParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterX = "x";
    static const std::string parameterZ = "z";

    static const std::string parameterReceiversTopLeft = "receiversTopLeft";
    double receiversTopLeftX = jsonFile[parameterReceiversTopLeft][parameterX];
    double receiversTopLeftZ = jsonFile[parameterReceiversTopLeft][parameterZ];

    static const std::string parameterReceiversBottomRight = "receiversBottomRight";
    double receiversBottomRightX = jsonFile[parameterReceiversBottomRight][parameterX];
    double receiversBottomRightZ = jsonFile[parameterReceiversBottomRight][parameterZ];

    if(receiversTopLeftX > receiversBottomRightX || receiversTopLeftZ > receiversBottomRightZ)
    {
        throw std::invalid_argument("Invald ranges for receivers in genericInput.json.");
    }

    jsonInput.receiversTopLeftCornerInM = {receiversTopLeftX, receiversTopLeftZ};
    jsonInput.receiversBottomRightCornerInM = {receiversBottomRightX, receiversBottomRightZ};
}

void genericInputCardReader::readGridOriginalParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterX = "x";
    static const std::string parameterZ = "z";

    static const std::string parameterNGridOriginal = "ngrid_original";
    int nGridOriginalX = jsonFile[parameterNGridOriginal][parameterX];
    int nGridOriginalZ = jsonFile[parameterNGridOriginal][parameterZ];

    if(nGridOriginalX <= 0 || nGridOriginalZ <= 0)
    {
        throw std::invalid_argument("Invalid grid_original input in genericInput.json.");
    }

    jsonInput.nGridOriginal = {nGridOriginalX, nGridOriginalZ};
}

void genericInputCardReader::readGridParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterX = "x";
    static const std::string parameterZ = "z";

    static const std::string parameterNGrid = "ngrid";

    int nGridX = jsonFile[parameterNGrid][parameterX];
    int nGridZ = jsonFile[parameterNGrid][parameterZ];

    if(nGridX < 3 || nGridZ < 3)
    {
        throw std::invalid_argument("Invalid grid input in genericInput.json.");
    }

    jsonInput.nGrid = {nGridX, nGridZ};
}

void genericInputCardReader::readNSourcesParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterNSources = "nSources";
    int nSources = jsonFile[parameterNSources];

    if(nSources <= 1)
    {
        throw std::invalid_argument("Invalid number of sources (" + std::to_string(nSources) + "<= 1) in genericInput.json.");
    }

    jsonInput.nSources = nSources;
}

void genericInputCardReader::readNReceiversParameter(const nlohmann::json &jsonFile, genericInput &jsonInput)
{
    static const std::string parameterNReceivers = "nReceivers";
    int nReceivers = jsonFile[parameterNReceivers];

    if(nReceivers <= 1)
    {
        throw std::invalid_argument("Invalid number of receivers in genericInput.json.");
    }

    jsonInput.nReceivers = nReceivers;
}
