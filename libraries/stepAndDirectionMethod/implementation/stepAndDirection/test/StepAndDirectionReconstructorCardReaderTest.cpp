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

            const std::string testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string inputFolder = testFolder + "input/";
            const std::string filename = "StepAndDirectionInputTest.json";
            const std::string filePath = inputFolder + filename;

            void SetUp() override {}

            void TearDown() override
            {
                if(remove((filePath).c_str()) != 0)
                {
                    perror("Error deleting StepAndDirectionInput file");
                }
            }

            std::string generateJsonWithInputParameters(const Parameters &reconstructorParameters, const Parameters &stepSizeParameters,
                const Parameters &directionParameters, const Parameters &doRegParameters)
            {
                std::stringstream s;
                s << "{\n";
                addToJson("ReconstructorParameters", reconstructorParameters, s);
                s << ",\n";
                addToJson("StepSizeParameters", stepSizeParameters, s);
                s << ",\n";
                addToJson("DirectionParameters", directionParameters, s);
                if(!doRegParameters.empty())
                {
                    s << ",\n";
                    s << "\t\t\"" << doRegParameters.begin()->first << "\":" << doRegParameters.begin()->second;
                }
                s << "}";
                return s.str();
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
                inputFile.open(filePath);
                inputFile << jsonInputString << std::endl;
                inputFile.close();
            }
        };

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, Constructor_ValidInput)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act
            StepAndDirectionReconstructorInputCardReader StepAndDirectionReader(testFolder, filename);
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

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, InvalidTolerance_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "-0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "-10"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, MissingTolerance_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, MissingInitialChi_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "-0.01"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, InvalidMaxIterationNumber_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "-10"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, MissingMaxIterationNumber_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, InvalidInitialStepSize_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.0"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, MissingInitialStepSize_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, InvalidSlope_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "-1"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, MissingSlope_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, InvalidDerivativeStepSize_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "-0.1"}};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, MissingDerivativeStepSize_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{};
            Parameters doRegParameters{{"DoConjugateGradientRegularisation", "true"}};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(StepAndDirectionReconstructorInputCardReaderTest, MissingDoConjugateGradientRegularisation_ExpectThrow)
        {
            // Arrange
            Parameters reconstructorParameters{{"Tolerance", "0.01"}, {"InitialChi", "0.001"}, {"MaxIterationNumber", "20"}};
            Parameters stepSizeParameters{{"InitialStepSize", "0.1"}, {"Slope", "0.00"}};
            Parameters directionParameters{{"DerivativeStepSize", "0.001"}};
            Parameters doRegParameters{};

            auto jsonInput = generateJsonWithInputParameters(reconstructorParameters, stepSizeParameters, directionParameters, doRegParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder, filename), std::invalid_argument);
        }

    }   // namespace inversionMethods
}   // namespace fwi
