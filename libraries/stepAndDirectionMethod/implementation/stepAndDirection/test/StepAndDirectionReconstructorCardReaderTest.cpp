#include "StepAndDirectionReconstructorInputCardReader.h"
#include <gtest/gtest.h>
#include <map>

namespace fwi
{
    namespace inversionMethods
    {
        class StepAndDirectionReconstructorInputCardReaderTest : public ::testing::Test
        {
        protected:
            using Parameters = std::map<std::string, std::string>;
            using ParametersCollection = std::map<std::string, Parameters>;

            const std::string _testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string _inputFolder = _testFolder + "input/";
            const std::string _filename = "StepAndDirectionInputTest.json";
            const std::string _filepath = _inputFolder + _filename;

            ParametersCollection _groupParameters{{"ReconstructorParameters", {{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}}},
                {"StepSizeParameters", {{"InitialStepSize", "0.1"}, {"Slope", "0.00"}}}, {"DirectionParameters", {{"DerivativeStepSize", "0.001"}}}};

            Parameters _singleParameters{{"DoConjugateGradientRegularisation", "true"}};

            std::string _jsonInput;

            void SetUp() override { _jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters); }

            void TearDown() override
            {
                if(remove((_filepath).c_str()) != 0)
                {
                    perror("Error deleting StepAndDirectionInput file");
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
                    for(const auto &param : groupParameters)
                    {
                        addToJson(param.first, param.second, stream);
                        if(&param != &*groupParameters.rbegin())
                        {
                            stream << ",";
                        }
                        stream << "\n";
                    }
                }
                stream << "}";
                return stream.str();
            }

            void addToJson(const std::string &name, const std::string &param, std::stringstream &stream) { stream << "\t\"" << name << "\": " << param; }

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

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_ValidInput)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            StepAndDirectionReconstructorInputCardReader StepAndDirectionReader(_testFolder, _filename);
            StepAndDirectionReconstructorInput input = StepAndDirectionReader.getInput();

            // Assert
            ASSERT_DOUBLE_EQ(0.01, input.reconstructorParameters.tolerance);
            ASSERT_DOUBLE_EQ(0.001, input.reconstructorParameters.startingChi);
            EXPECT_EQ(20, input.reconstructorParameters.maxIterationsNumber);

            ASSERT_DOUBLE_EQ(0.1, input.stepSizeParameters.initialStepSize);
            ASSERT_DOUBLE_EQ(0.0, input.stepSizeParameters.slope);

            ASSERT_DOUBLE_EQ(0.001, input.directionParameters.derivativeStepSize);

            ASSERT_TRUE(input.doConjugateGradientRegularisation);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_NegativeTolerance_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("ReconstructorParameters").at("Tolerance") = "-0.01";
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_MissingTolerance_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("ReconstructorParameters").erase("Tolerance");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_MissingInitialChi_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("ReconstructorParameters").erase("InitialChi");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_NegativeMaxIterationNumber_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("ReconstructorParameters").at("MaxIterationNumber") = "-1";
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_MissingMaxIterationNumber_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("ReconstructorParameters").erase("MaxIterationNumber");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_ZeroInitialStepSize_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("StepSizeParameters").at("InitialStepSize") = "0.0";
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_MissingInitialStepSize_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("StepSizeParameters").erase("InitialStepSize");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_NegativeSlope_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("StepSizeParameters").at("Slope") = "-1";
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_MissingSlope_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("StepSizeParameters").erase("Slope");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_NegativeDerivativeStepSize_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("DirectionParameters").at("DerivativeStepSize") = "-0.1";
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_MissingDerivativeStepSize_ExceptionThrown)
        {
            // Arrange
            _groupParameters.at("DirectionParameters").erase("DerivativeStepSize");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, constructor_MissingDoConjugateGradientRegularisation_ExceptionThrown)
        {
            _singleParameters.erase("DoConjugateGradientRegularisation");
            auto jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(_testFolder, _filename), std::invalid_argument);
        }
    }   // namespace inversionMethods
}   // namespace fwi
