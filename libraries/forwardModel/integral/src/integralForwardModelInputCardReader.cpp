#include <iostream>

#include "integralForwardModelInputCardReader.h"
#include "json.h"

namespace fwi
{
    namespace forwardModels
    {
        integralForwardModelInputCardReader::integralForwardModelInputCardReader(const std::string &caseFolder, const std::string &filename)
            : io::inputCardReader()
            , _fileName(filename)
        {
            const std::string stringInputFolder = "/input/";
            std::string filePath = caseFolder + stringInputFolder + _fileName;
            readJsonFile(filePath);
        }

        void integralForwardModelInputCardReader::readJsonFile(const std::string &filePath)
        {
            nlohmann::json jsonFile = readFile(filePath);
            readIterParameters(jsonFile);
        }

        void integralForwardModelInputCardReader::readIterParameters(const nlohmann::json &jsonFile)
        {
            const std::string parameterIter = "Iter2";
            const std::string parameterNumber = "n";
            const std::string parameterTolerance = "tolerance";
            const std::string parameterCalcAlpha = "calcAlpha";

            nlohmann::json iterObject = io::ReadJsonHelper::tryGetParameterFromJson<nlohmann::json>(jsonFile, _fileName, parameterIter);

            int nrOfIterations = io::ReadJsonHelper::tryGetParameterFromJson<int>(iterObject, _fileName, parameterNumber);
            if(nrOfIterations <= 0)
            {
                throw std::invalid_argument("Invalid number of iterations in IntegralFMInput.json.");
            }

            double tolerance = io::ReadJsonHelper::tryGetParameterFromJson<double>(iterObject, _fileName, parameterTolerance);
            if(tolerance <= 0)
            {
                throw std::invalid_argument("Invalid tolerance in IntegralFMInput.json.");
            }

            bool calcAlpha = io::ReadJsonHelper::tryGetParameterFromJson<bool>(iterObject, _fileName, parameterCalcAlpha);

            CostFunction costFunction = readCostFunctionParameters(iterObject);

            _input = integralForwardModelInput(nrOfIterations, tolerance, calcAlpha, costFunction);
        }

        CostFunction integralForwardModelInputCardReader::readCostFunctionParameters(nlohmann::json &iterObject)
        {
            const std::string parameterCostFunction = "CostFunction";

            std::string costFunctionInput = io::ReadJsonHelper::tryGetParameterFromJson<std::string>(iterObject, _fileName, parameterCostFunction);

            std::map<std::string, CostFunction> costFunctionStringMap{std::make_pair("leastSquares", leastSquares)};

            CostFunction costFunction;
            try
            {
                costFunction = costFunctionStringMap.at(costFunctionInput);
            }
            catch(const std::out_of_range &e)
            {
                throw std::invalid_argument("Invalid cost function in input file for integral forward model.");
            }

            return costFunction;
        }
    }   // namespace forwardModels
}   // namespace fwi
