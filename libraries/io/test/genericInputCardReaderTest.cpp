#include "genericInputCardReader.h"
#include "genericInput.h"
#include "utilityFunctions.h"
#include <gtest/gtest.h>
#include <ostream>

namespace fwi
{
    namespace io
    {
        class genericInputCardReaderTest : public ::testing::Test
        {
        protected:
            using Parameters = std::map<std::string, std::string>;
            using ParametersCollection = std::map<std::string, Parameters>;

            const std::string _testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string _inputFolder = _testFolder + "input/";
            const std::string _filename = "GenericInputTest.json";
            const std::string _filePath = _inputFolder + _filename;

            ParametersCollection groupParameters{{"Freq", {{"min", "10.0"}, {"max", "40.0"}, {"nTotal", "15"}}},
                {"reservoirTopLeft", {{"x", "-300.0"}, {"z", "0.0"}}}, {"reservoirBottomRight", {{"x", "300.0"}, {"z", "300.0"}}},
                {"sourcesTopLeft", {{"x", "-480.0"}, {"z", "-5.0"}}}, {"sourcesBottomRight", {{"x", "480"}, {"z", "-5.0"}}},
                {"receiversTopLeft", {{"x", "-480.0"}, {"z", "-5.0"}}}, {"receiversBottomRight", {{"x", "480"}, {"z", "-5.0"}}},
                {"ngrid_original", {{"x", "64"}, {"z", "32"}}}, {"ngrid", {{"x", "64"}, {"z", "32"}}}};

            Parameters singleParameters{{"c_0", "2000.0"}, {"verbosity", "false"}, {"nSources", "17"}, {"nReceivers", "17"}, {"fileName", "\"temple\""}};

            std::string jsonInput;

            void SetUp() override { jsonInput = generateJsonWithInputParameters(groupParameters, singleParameters); }

            void TearDown() override
            {
                if(remove(_filePath.c_str()) != 0)
                {
                    perror("Error deleting GenericInput file");
                }
            }

            void writeInputFile(const std::string &jsonInputString) const
            {
                std::ofstream inputFile;
                inputFile.open(_filePath);
                inputFile << jsonInputString << std::endl;
                inputFile.close();
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
        };

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_c0)
        {
            // Arrange
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);

            // Assert
            ASSERT_DOUBLE_EQ(reader.getInput().c0, 2000.0);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_Freq)
        {
            // Arrange
            core::freqInfo expectedFreq = {10, 40, 15};
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);
            auto actualFreq = reader.getInput().freq;

            // Assert
            ASSERT_DOUBLE_EQ(actualFreq.min, expectedFreq.min);
            ASSERT_DOUBLE_EQ(actualFreq.max, expectedFreq.max);
            ASSERT_DOUBLE_EQ(actualFreq.nTotal, expectedFreq.nTotal);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_ReservoirCoordinates)
        {
            // Arrange
            std::array<double, 2> expectedTopLeft{-300.0, 0.0};
            std::array<double, 2> expectedBottomRight{300.0, 300.0};
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);
            auto actualTopLeft = reader.getInput().reservoirTopLeftCornerInM;
            auto actualBottomRight = reader.getInput().reservoirBottomRightCornerInM;

            // Assert
            ASSERT_DOUBLE_EQ(expectedTopLeft[0], actualTopLeft[0]);
            ASSERT_DOUBLE_EQ(expectedTopLeft[1], actualTopLeft[1]);

            ASSERT_DOUBLE_EQ(expectedBottomRight[0], actualBottomRight[0]);
            ASSERT_DOUBLE_EQ(expectedBottomRight[1], actualBottomRight[1]);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_SourceCoordinates)
        {
            // Arrange
            std::array<double, 2> expectedTopLeft{-480.0, -5.0};
            std::array<double, 2> expectedBottomRight{480.0, -5.0};
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);
            auto actualTopLeft = reader.getInput().sourcesTopLeftCornerInM;
            auto actualBottomRight = reader.getInput().sourcesBottomRightCornerInM;

            // Assert
            ASSERT_DOUBLE_EQ(expectedTopLeft[0], actualTopLeft[0]);
            ASSERT_DOUBLE_EQ(expectedTopLeft[1], actualTopLeft[1]);

            ASSERT_DOUBLE_EQ(expectedBottomRight[0], actualBottomRight[0]);
            ASSERT_DOUBLE_EQ(expectedBottomRight[1], actualBottomRight[1]);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_ReceiverCoordinates)
        {
            // Arrange
            std::array<double, 2> expectedTopLeft{-480.0, -5.0};
            std::array<double, 2> expectedBottomRight{480.0, -5.0};
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);
            auto actualTopLeft = reader.getInput().receiversTopLeftCornerInM;
            auto actualBottomRight = reader.getInput().receiversBottomRightCornerInM;

            // Assert
            ASSERT_DOUBLE_EQ(expectedTopLeft[0], actualTopLeft[0]);
            ASSERT_DOUBLE_EQ(expectedTopLeft[1], actualTopLeft[1]);

            ASSERT_DOUBLE_EQ(expectedBottomRight[0], actualBottomRight[0]);
            ASSERT_DOUBLE_EQ(expectedBottomRight[1], actualBottomRight[1]);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_NumberOfSourcesAndReceivers)
        {
            // Arrange
            int expectedNumberOfSources = 17;
            int expectedNumberOfReceivers = 17;
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);

            // Assert
            ASSERT_EQ(reader.getInput().nSources, expectedNumberOfSources);
            ASSERT_EQ(reader.getInput().nReceivers, expectedNumberOfReceivers);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_NumberOfGridPoints)
        {
            // Arrange
            std::array<int, 2> expectedNumberOfGridPoints{64, 32};
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);

            // Assert
            ASSERT_EQ(reader.getInput().nGrid[0], expectedNumberOfGridPoints[0]);
            ASSERT_EQ(reader.getInput().nGrid[1], expectedNumberOfGridPoints[1]);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_NumberOfOriginalGridPoints)
        {
            // Arrange
            std::array<int, 2> expectedNumberOfOriginalGridPoints{64, 32};
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);

            // Assert
            ASSERT_EQ(reader.getInput().nGridOriginal[0], expectedNumberOfOriginalGridPoints[0]);
            ASSERT_EQ(reader.getInput().nGridOriginal[1], expectedNumberOfOriginalGridPoints[1]);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_verbosity)
        {
            // Arrange
            bool expectedVerbosity = false;
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);

            // Assert
            EXPECT_EQ(reader.getInput().verbose, expectedVerbosity);
        }

        TEST_F(genericInputCardReaderTest, constructor_ValidInput_filename)
        {
            // Arrange
            std::string expectedFilename = "temple";
            writeInputFile(jsonInput);

            // Act
            genericInputCardReader reader(_testFolder, _filename);

            // Assert
            EXPECT_EQ(reader.getInput().fileName, expectedFilename);
        }

        TEST_F(genericInputCardReaderTest, constructor_MissingC0_ExceptionsThrown)
        {
            // Arrange
            singleParameters.erase("c_0");
            jsonInput = generateJsonWithInputParameters(groupParameters, singleParameters);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(genericInputCardReader reader(_testFolder, _filename), std::invalid_argument);
        }

    }   // namespace io
}   // namespace fwi
