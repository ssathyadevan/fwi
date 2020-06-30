#include <iostream>

#include "finiteDifferenceForwardModelInputCardReader.h"
#include "json.h"

namespace fwi
{
    namespace forwardModels
    {
        finiteDifferenceForwardModelInputCardReader::finiteDifferenceForwardModelInputCardReader(const std::string &caseFolder)
            : io::inputCardReader()
        {
            const std::string stringInputFolder = "/input/";
            std::string filePath = caseFolder + stringInputFolder + _fileName;
            readJsonFile(filePath);
        }

        void finiteDifferenceForwardModelInputCardReader::readJsonFile(const std::string &filePath)
        {
            nlohmann::json jsonFile = readFile(filePath);
            readPMLWidthFactorParameters(jsonFile);
            readSourceParameters(jsonFile);
            readCostFunctionParameters(jsonFile);
        }

        void finiteDifferenceForwardModelInputCardReader::readPMLWidthFactorParameters(const nlohmann::json &jsonFile)
        {
            const std::string parameterPMLWidthFactor = "PMLWidthFactor";
            const std::string parameterX = "x";
            const std::string parameterZ = "z";
            nlohmann::json iterObject = io::ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, _fileName, parameterPMLWidthFactor);

            double x = io::ReadJsonHelper::tryGetParameterFromJson<double>(iterObject, _fileName, parameterX);
            double z = io::ReadJsonHelper::tryGetParameterFromJson<double>(iterObject, _fileName, parameterZ);

            _input.pmlWidthFactor = PMLWidthFactor(x, z);
        }

        void finiteDifferenceForwardModelInputCardReader::readSourceParameters(const nlohmann::json &jsonFile)
        {
            const std::string parameterSource = "SourceParameter";
            const std::string parameterHalfWidth = "r";
            const std::string parameterShape = "beta";

            nlohmann::json iterObject = io::ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, _fileName, parameterSource);

            double halfWidth = io::ReadJsonHelper::tryGetParameterFromJson<double>(iterObject, _fileName, parameterHalfWidth);
            double shape = io::ReadJsonHelper::tryGetParameterFromJson<double>(iterObject, _fileName, parameterShape);

            _input.sourceParameter = SourceParameter(halfWidth, shape);
        }

        void finiteDifferenceForwardModelInputCardReader::readCostFunctionParameters(const nlohmann::json &jsonFile)
        {
            const std::string parameterCostFunction = "CostFunction";
            std::string costFunctionString = io::ReadJsonHelper::tryGetParameterFromJson<std::string>(jsonFile, _fileName, parameterCostFunction);
            std::map<std::string, CostFunction> costFunctionStringMap{std::make_pair("leastSquares", leastSquares)};
            _input.costFunction = costFunctionStringMap.at(costFunctionString);
        }
    }   // namespace forwardModels
}   // namespace fwi
