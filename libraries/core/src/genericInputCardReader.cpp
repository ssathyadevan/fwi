#include "genericInputCardReader.h"
#include "json.h"
#include <exception>
#include <iostream>
#include <string>

genericInputCardReader::genericInputCardReader(const std::string &caseFolder_)
{
    genericInput input;
    std::string filePath = setFolders(caseFolder_, _fileName, input);
    readJsonFile(filePath, _fileName, input);
    _input = input;
}

std::string genericInputCardReader::setFolders(const std::string &caseFolderWithSlash, const std::string &fileName, genericInput &jsonInput)
{
    const std::string stringInputFolder = "/input/";
    const std::string stringOutputFolder = "/output/";

    std::string caseFolder = removeLastSlash(caseFolderWithSlash);
    std::string runName = getRunName(caseFolder);

    jsonInput.caseFolder = caseFolder;
    jsonInput.runName = runName;
    jsonInput.inputFolder = caseFolder + stringInputFolder;
    jsonInput.outputLocation = caseFolder + stringOutputFolder;

    std::string filePath = caseFolder + stringInputFolder + fileName;
    return filePath;
}

void genericInputCardReader::readJsonFile(const std::string &filePath, const std::string &fileName, genericInput &input)
{
    nlohmann::json jsonFile = readFile(filePath);

    readC0Parameter(jsonFile, fileName, input);
    readFreqParameter(jsonFile, fileName, input);
    readReservoirParameter(jsonFile, fileName, input);
    readSourcesParameter(jsonFile, fileName, input);
    readReceiversParameter(jsonFile, fileName, input);
    readGridOriginalParameter(jsonFile, fileName, input);
    readGridParameter(jsonFile, fileName, input);
    readNSourcesParameter(jsonFile, fileName, input);
    readNReceiversParameter(jsonFile, fileName, input);

    const std::string parameterFileName = "fileName";
    input.fileName = ReadJsonHelper::tryGetParameterFromJson<std::string>(jsonFile, fileName, parameterFileName);

    const std::string parameterVerbose = "verbosity";

    input.verbose = ReadJsonHelper::tryGetParameterFromJson<bool>(jsonFile, fileName, parameterVerbose);
}

void genericInputCardReader::readXZParametersFromJsonObject(const nlohmann::json &jsonFile, const std::string &fileName, std::array<double, 2> &array)
{
    const std::string parameterX = "x";
    double xValue = ReadJsonHelper::tryGetParameterFromJson<double>(jsonFile, fileName, parameterX);

    const std::string parameterZ = "z";
    double zValue = ReadJsonHelper::tryGetParameterFromJson<double>(jsonFile, fileName, parameterZ);

    array = {xValue, zValue};
}

void genericInputCardReader::readXZParametersFromJsonObject(const nlohmann::json &jsonFile, const std::string &fileName, std::array<int, 2> &array)
{
    const std::string parameterX = "x";
    int xValue = ReadJsonHelper::tryGetParameterFromJson<int>(jsonFile, fileName, parameterX);

    const std::string parameterZ = "z";
    int zValue = ReadJsonHelper::tryGetParameterFromJson<int>(jsonFile, fileName, parameterZ);

    array = {xValue, zValue};
}

void genericInputCardReader::readC0Parameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterC0 = "c_0";
    double c0 = ReadJsonHelper::tryGetParameterFromJson<double>(jsonFile, fileName, parameterC0);
    if(c0 <= 0)
    {
        throw std::invalid_argument("Invalid value for c0 (" + std::to_string(c0) + " <= 0) in: " + fileName);
    }
    jsonInput.c0 = c0;
}

void genericInputCardReader::readFreqParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterFreq = "Freq";
    const std::string parameterFreqMin = "min";
    const std::string parameterFreqMax = "max";
    const std::string parameterFreqNTotal = "nTotal";

    nlohmann::json freqJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterFreq);

    double min = ReadJsonHelper::tryGetParameterFromJson<double>(freqJsonObject, fileName, parameterFreqMin);
    double max = ReadJsonHelper::tryGetParameterFromJson<double>(freqJsonObject, fileName, parameterFreqMax);
    if(min >= max || min <= 0)
    {
        throw std::invalid_argument(
            "Invalid ranges for frequenties (" + std::to_string(min) + " >= " + std::to_string(max) + ") (" + std::to_string(min) + " <= 0) in: " + fileName);
    }

    double nTotal = ReadJsonHelper::tryGetParameterFromJson<double>(freqJsonObject, fileName, parameterFreqNTotal);
    if(nTotal <= 1)
    {
        throw std::invalid_argument("Invalid number of frequenties (" + parameterFreqNTotal + " <= " + std::to_string(nTotal) + ") in: " + fileName);
    }
    jsonInput.freq = freqInfo(min, max, nTotal);
}

void genericInputCardReader::readReservoirParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterReservoirTopLeft = "reservoirTopLeft";
    const std::string parameterReservoirBottomRight = "reservoirBottomRight";

    std::array<double, 2> topLeftArray;
    nlohmann::json topLeftJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterReservoirTopLeft);
    readXZParametersFromJsonObject(topLeftJsonObject, fileName, topLeftArray);

    std::array<double, 2> bottemRightArray;
    nlohmann::json bottomRightJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterReservoirBottomRight);
    readXZParametersFromJsonObject(bottomRightJsonObject, fileName, bottemRightArray);

    if(topLeftArray[0] > bottemRightArray[0] || topLeftArray[1] > bottemRightArray[1])
    {
        throw std::invalid_argument("Invalid ranges for " + parameterReservoirTopLeft + " and " + parameterReservoirBottomRight + "(" +
                                    std::to_string(topLeftArray[0]) + " > " + std::to_string(bottemRightArray[0]) + "(" + std::to_string(topLeftArray[1]) +
                                    " > " + std::to_string(bottemRightArray[1]) + " in: " + fileName);
    }
    jsonInput.reservoirTopLeftCornerInM = topLeftArray;
    jsonInput.reservoirBottomRightCornerInM = bottemRightArray;
}

void genericInputCardReader::readSourcesParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterSourcesTopLeft = "sourcesTopLeft";
    const std::string parameterSourcesBottomRight = "sourcesBottomRight";

    std::array<double, 2> topLeftArray;
    nlohmann::json topLeftJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterSourcesTopLeft);
    readXZParametersFromJsonObject(topLeftJsonObject, fileName, topLeftArray);

    std::array<double, 2> bottemRightArray;
    nlohmann::json bottomRightJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterSourcesBottomRight);
    readXZParametersFromJsonObject(bottomRightJsonObject, fileName, bottemRightArray);

    if(topLeftArray[0] > bottemRightArray[0] || topLeftArray[1] > bottemRightArray[1])
    {
        throw std::invalid_argument("Invalid ranges for " + parameterSourcesTopLeft + " and " + parameterSourcesBottomRight + "(" +
                                    std::to_string(topLeftArray[0]) + " > " + std::to_string(bottemRightArray[0]) + "(" + std::to_string(topLeftArray[1]) +
                                    " > " + std::to_string(bottemRightArray[1]) + " in: " + fileName);
    }
    jsonInput.sourcesTopLeftCornerInM = topLeftArray;
    jsonInput.sourcesBottomRightCornerInM = bottemRightArray;
}

void genericInputCardReader::readReceiversParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterReceiversTopLeft = "receiversTopLeft";
    const std::string parameterReceiversBottomRight = "receiversBottomRight";

    std::array<double, 2> topLeftArray;
    nlohmann::json topLeftJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterReceiversTopLeft);
    readXZParametersFromJsonObject(topLeftJsonObject, fileName, topLeftArray);

    std::array<double, 2> bottemRightArray;
    nlohmann::json bottomRightJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterReceiversBottomRight);
    readXZParametersFromJsonObject(bottomRightJsonObject, fileName, bottemRightArray);

    if(topLeftArray[0] > bottemRightArray[0] || topLeftArray[1] > bottemRightArray[1])
    {
        throw std::invalid_argument("Invalid ranges for " + parameterReceiversTopLeft + " and " + parameterReceiversBottomRight + "(" +
                                    std::to_string(topLeftArray[0]) + " > " + std::to_string(bottemRightArray[0]) + "(" + std::to_string(topLeftArray[1]) +
                                    " > " + std::to_string(bottemRightArray[1]) + " in: " + fileName);
    }

    jsonInput.receiversTopLeftCornerInM = topLeftArray;
    jsonInput.receiversBottomRightCornerInM = bottemRightArray;
}

void genericInputCardReader::readGridOriginalParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterNGridOriginal = "ngrid_original";

    std::array<int, 2> nGridOriginalArray;
    nlohmann::json nGridOriginalJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterNGridOriginal);
    readXZParametersFromJsonObject(nGridOriginalJsonObject, fileName, nGridOriginalArray);

    if(nGridOriginalArray[0] <= 0 || nGridOriginalArray[1] <= 0)
    {
        throw std::invalid_argument("Grid original size (" + std::to_string(nGridOriginalArray[0]) + "," + std::to_string(nGridOriginalArray[1]) +
                                    ") is invalid because (x > 3, z > 3) is required in  " + fileName);
    }
    jsonInput.nGridOriginal = nGridOriginalArray;
}

void genericInputCardReader::readGridParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterNGrid = "ngrid";

    std::array<int, 2> nGridArray;
    nlohmann::json nGridJsonObject = ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, fileName, parameterNGrid);
    readXZParametersFromJsonObject(nGridJsonObject, fileName, nGridArray);

    if(nGridArray[0] < 3 || nGridArray[1] < 3)
    {
        throw std::invalid_argument("Grid size (" + std::to_string(nGridArray[0]) + "," + std::to_string(nGridArray[1]) +
                                    ") is invalid because (x > 3, z > 3) is required in  " + fileName);
    }
    jsonInput.nGrid = nGridArray;
}

void genericInputCardReader::readNSourcesParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterNSources = "nSources";
    int nSources = ReadJsonHelper::tryGetParameterFromJson<int>(jsonFile, fileName, parameterNSources);
    if(nSources <= 1)
    {
        throw std::invalid_argument("Invalid number of sources (" + std::to_string(nSources) + "<= 1) in: " + fileName);
    }
    jsonInput.nSources = nSources;
}

void genericInputCardReader::readNReceiversParameter(const nlohmann::json &jsonFile, const std::string &fileName, genericInput &jsonInput)
{
    const std::string parameterNReceivers = "nReceivers";
    int nReceivers = ReadJsonHelper::tryGetParameterFromJson<int>(jsonFile, fileName, parameterNReceivers);
    if(nReceivers <= 1)
    {
        throw std::invalid_argument("Invalid number of receivers (" + std::to_string(nReceivers) + "<= 1) in: " + fileName);
    }
    jsonInput.nReceivers = nReceivers;
}
