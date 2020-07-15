#pragma once

#include "StepAndDirectionReconstructorInput.h"
#include "inputCardReader.h"
#include <fstream>
#include <sstream>
#include <string>

namespace fwi
{
    namespace inversionMethods
    {
        class StepAndDirectionReconstructorInputCardReader : public io::inputCardReader
        {
        public:
            StepAndDirectionReconstructorInputCardReader(const std::string &caseFolder, const std::string &filename = "StepAndDirectionInput.json");
            const StepAndDirectionReconstructorInput getInput() const { return _input; }

        private:
            const std::string _fileName;
            void readJsonFile(const std::string &filePath);

            void readReconstructorParameters(const nlohmann::json &jsonFile);
            void readStepSizeParameters(const nlohmann::json &jsonFile);
            void readDirectionParameters(const nlohmann::json &jsonFile);

            StepAndDirectionReconstructorInput _input;
        };
    }   // namespace inversionMethods
}   // namespace fwi
