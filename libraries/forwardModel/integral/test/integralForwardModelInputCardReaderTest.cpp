#include "integralForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include <gtest/gtest.h>
#include <map>

namespace fwi
{
    namespace forwardModels
    {
        class integralForwardModelInputCardReaderTest : public ::testing::Test
        {
        protected:
            using Parameters = std::map<std::string, std::string>;
            using ParametersCollection = std::map<std::string, Parameters>;

            const std::string _testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string _inputFolder = _testFolder + "input/";
            const std::string _filename = "IntegralFMInputTest.json";
            const std::string _filepath = _inputFolder + _filename;

            ParametersCollection _parameters{{"Iter2", {{"n", "15"}, {"tolerance", "5.0e-5"}, {"calcAlpha", "false"}}}};

            std::string _jsonInput;

            void SetUp() override { _jsonInput = generateJsonWithInputParameters(_parameters); }
            void TearDown() override
            {
                if(remove((_filepath).c_str()) != 0)
                {
                    perror("Error deleting integralFMInput file");
                }
            }

            std::string generateJsonWithInputParameters(const ParametersCollection &parameters)
            {
                std::stringstream stream;
                stream << "{\n";

                if(!parameters.empty())
                {
                    for(const auto &param : parameters)
                    {
                        addToJson(param.first, param.second, stream);
                        if(&param != &*parameters.rbegin())
                        {
                            stream << ",";
                        }
                        stream << "\n";
                    }
                }
                stream << "}";
                return stream.str();
            }

            void addToJson(const std::string &name, const Parameters &params, std::stringstream &stream)
            {
                stream << "\t\"" << name << "\": { \n";
                for(const auto &param : params)
                {
                    stream << "\t\t\"" << param.first << "\":" << param.second;
                    // If not last element in the map add comma.
                    if(&param != &*params.rbegin())
                    {
                        stream << ",";
                    }
                    stream << "\n";
                }
                stream << "\t }";
            }

            void writeInputFile(const std::string &jsonInputString) const
            {
                std::ofstream inputFile;
                inputFile.open(_filepath);
                inputFile << jsonInputString << std::endl;
                inputFile.close();
            }
        };

        TEST_F(integralForwardModelInputCardReaderTest, constructor_ValidInput_Iter1)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            integralForwardModelInputCardReader integralReader(_testFolder, _filename);
            integralForwardModelInput input = integralReader.getInput();

            // Assert
            EXPECT_EQ(15, input.nrOfIterations);
            ASSERT_DOUBLE_EQ(5.0e-5, input.tolerance);
            ASSERT_FALSE(input.calcAlpha);
            EXPECT_EQ(leastSquares, input.costFunction);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_ZeroN_ExceptionThrown)
        {
            // Arrange
            _parameters.at("Iter2").at("n") = "0";
            _jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(_jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_NegativeTolerance_ExceptionThrown)
        {
            // Arrange
            _parameters.at("Iter2").at("tolerance") = "-1.0";
            auto jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_MissingN_ExceptionThrown)
        {
            // Arrange
            _parameters.at("Iter2").erase("n");
            auto jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_MissingTolerance_ExceptionThrown)
        {
            // Arrange
            _parameters.at("Iter2").erase("tolerance");
            auto jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(_testFolder, _filename), std::invalid_argument);
        }
        TEST_F(integralForwardModelInputCardReaderTest, constructor_MissingCalcAlpha_ExceptionThrown)
        {
            // Arrange
            Parameters iter2{{"n", "5"}, {"tolerance", "5.0e-5"}};
            _parameters.at("Iter2").erase("calcAlpha");
            auto jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(_testFolder, _filename), std::invalid_argument);
        }
    }   // namespace forwardModels
}   // namespace fwi
