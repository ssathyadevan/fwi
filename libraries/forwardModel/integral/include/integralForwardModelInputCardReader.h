#pragma once
#include "ReadJsonHelper.h"
#include "inputCardReader.h"
#include "integralForwardModelInput.h"
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

namespace fwi
{
    namespace forwardModels
    {
        class integralForwardModelInputCardReader : public io::inputCardReader
        {
        public:
            integralForwardModelInputCardReader(const std::string &caseFolder);
            const integralForwardModelInput getInput() const { return _input; }

        private:
            const std::string _fileName = "IntegralFMInput.json";
            integralForwardModelInput _input;
            void readJsonFile(const std::string &filePath);
            void readIterParameters(const nlohmann::json &jsonFile);
            CostFunction readCostFunctionParameters(nlohmann::json &iterObject);
        };
    }   // namespace forwardModels
}   // namespace fwi
