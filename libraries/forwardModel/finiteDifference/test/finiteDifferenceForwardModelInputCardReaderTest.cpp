#include "finiteDifferenceForwardModelInputCardReader.h"
#include "finiteDifferenceForwardModel.h"

#include <gtest/gtest.h>
#include <map>

namespace fwi
{
    namespace forwardModels
    {
        class finiteDifferenceForwardModelInputCardReaderTest : public ::testing::Test
        {
        protected:
            using Parameters = std::map<std::string, std::string>;
            using ParametersCollection = std::map<std::string, Parameters>;

            const std::string _testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string _inputFolder = _testFolder + "input/";
            const std::string _filename = "FiniteDifferenceFMInputTest.json";
            const std::string _filepath = _inputFolder + _filename;

            ParametersCollection _groupParameters{{"PMLWidthFactor", {{"x", "0.0"}, {"z", "0.0"}}}, {"SourceParameter", {{"r", "4"}, {"beta", "6.31"}}}};
            Parameters _singleParameters{{"CostFunction", "\"leastSquares\""}, {"boundaryConditionType", "\"FirstOrderABC\""}};

            std::string _jsonInput;

            void SetUp() override { _jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters); }

            void TearDown() override
            {
                if(remove((_filepath).c_str()) != 0)
                {
                    perror("Error deleting finiteDifferenceFMInput file");
                }
            }

            std::string generateJsonWithInputParameters(const ParametersCollection &groupParameters, const Parameters &singleParameters)
            {
                std::stringstream stream;
                stream << "{\n";
                if(!singleParameters.empty())
                {
                    for(const auto &param : singleParameters)
                    {
                        addToJson(param.first, param.second, stream);
                        stream << ",\n";
                    }
                }

                if(!groupParameters.empty())
                {
                    auto delim = "";
                    for(const auto &param : groupParameters)
                    {
                        stream << delim;
                        addToJson(param.first, param.second, stream);
                        delim = ",\n";
                    }
                }
                stream << "\n}";
                return stream.str();
            }

            void addToJson(const std::string &name, const std::string &param, std::stringstream &stream) { stream << "\t\"" << name << "\": " << param; }
            void addToJson(const std::string &name, const Parameters &params, std::stringstream &stream)
            {
                stream << "\t\"" << name << "\": { \n";
                auto delim = "";
                for(const auto &param : params)
                {
                    stream << delim;
                    stream << "\t\t\"" << param.first << "\":" << param.second;
                    delim = ",\n";
                }
                stream << " }";
            }

            void writeInputFile(const std::string &jsonInputString) const
            {
                std::ofstream inputFile;
                inputFile.open(_filepath);
                inputFile << jsonInputString << std::endl;
                inputFile.close();
            }
        };

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_ValidInput)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(_testFolder, _filename);
            finiteDifferenceForwardModelInput input = finiteDifferenceReader.getInput();

            // Assert
            ASSERT_DOUBLE_EQ(0.0, input.pmlWidthFactor.x);
            ASSERT_DOUBLE_EQ(0.0, input.pmlWidthFactor.z);

            EXPECT_EQ(4, input.sourceParameter.r);
            EXPECT_EQ(6.31, input.sourceParameter.beta);

            EXPECT_EQ(leastSquares, input.costFunction);
            EXPECT_EQ(BoundaryConditionType::FirstOrderABC, input.boundaryConditionType);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_missingX_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("PMLWidthFactor").erase("x");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_missingZ_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("PMLWidthFactor").erase("z");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_missingR_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("SourceParameter").erase("r");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_missingBeta_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("SourceParameter").erase("beta");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_InvalidCostFunction_ExceptionThrown)
        {
            // Arrange
            _singleParameters.at("CostFunction") = "\"InvalidCostFunction\"";
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_InvalidBoundaryCondition_ExceptionThrown)
        {
            // Arrange
            _singleParameters.at("boundaryConditionType") = "\"InvalidBoundaryCondition\"";
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(_testFolder, _filename), std::invalid_argument);
        }

    }   // namespace forwardModels
}   // namespace fwi
