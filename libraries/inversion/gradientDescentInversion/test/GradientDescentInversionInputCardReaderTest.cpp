#include "gradientDescentInversionInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        class GradientDescentInversionInputCardReaderTest : public ::testing::Test
        {
        protected:
            using Parameters = std::map<std::string, std::string>;

            const std::string _testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string _inputFolder = _testFolder + "input/";
            const std::string _filename = "GradientDescentInversionInputTest.json";
            const std::string _filePath = _inputFolder + _filename;

            Parameters _parameters{{"gamma0", "0.1"}, {"x0", "0.001"}, {"h", "0.001"}, {"iter", "20"}};

            std::string _jsonInput;

            void SetUp() override { _jsonInput = generateJsonWithInputParameters(_parameters); }

            void TearDown() override
            {
                if(remove(_filePath.c_str()) != 0)
                {
                    perror("Error deleting ConjugateGradientInversionInput file");
                }
            }

            void writeInputFile(const std::string &_jsonInputString) const
            {
                std::ofstream inputFile;
                inputFile.open(_filePath);
                inputFile << _jsonInputString << std::endl;
                inputFile.close();
            }

            std::string generateJsonWithInputParameters(const Parameters &parameters)
            {
                std::stringstream stream;
                stream << "{\n";
                if(!parameters.empty())
                {
                    for(const auto &param : parameters)
                    {
                        addToJson(param.first, param.second, stream);
                        // If not last element in the map add comma.
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

            void addToJson(const std::string &name, const std::string &param, std::stringstream &stream) { stream << "\t\"" << name << "\": " << param; }
        };

        TEST_F(GradientDescentInversionInputCardReaderTest, constructor_ValidInput_InputParameters)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            gradientDescentInversionInputCardReader gradientDescentReader(_testFolder, _filename);
            gradientDescentInversionInput input = gradientDescentReader.getInput();

            // Assert
            ASSERT_DOUBLE_EQ(0.1, input.gamma0);
            ASSERT_DOUBLE_EQ(0.001, input.x0);
            ASSERT_DOUBLE_EQ(0.001, input.h);
            EXPECT_EQ(20, input.iter);
        }

        TEST_F(GradientDescentInversionInputCardReaderTest, constructor_NegativeH_ExceptionsThrown)
        {
            // Arrange
            _parameters.at("h") = "-0.5";
            _jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(_jsonInput);

            // Act & Assert
            EXPECT_THROW(gradientDescentInversionInputCardReader gradientDescentReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(GradientDescentInversionInputCardReaderTest, constructor_NegativeIter_ExceptionsThrown)
        {
            // Arrange
            _parameters.at("iter") = "-20";

            _jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(_jsonInput);

            // Act & Assert
            EXPECT_THROW(gradientDescentInversionInputCardReader gradientDescentReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(GradientDescentInversionInputCardReaderTest, constructor_MissingGamma0_ExceptionsThrown)
        {
            // Arrange
            _parameters.erase("gamma0");

            _jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(_jsonInput);

            // Act & Assert
            EXPECT_THROW(gradientDescentInversionInputCardReader gradientDescentReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(GradientDescentInversionInputCardReaderTest, constructor_MissingX0_ExceptionsThrown)
        {
            // Arrange
            _parameters.erase("x0");

            _jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(_jsonInput);

            // Act & Assert
            EXPECT_THROW(gradientDescentInversionInputCardReader gradientDescentReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(GradientDescentInversionInputCardReaderTest, constructor_MissingH_ExceptionsThrown)
        {
            // Arrange
            _parameters.erase("h");

            _jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(_jsonInput);

            // Act & Assert
            EXPECT_THROW(gradientDescentInversionInputCardReader gradientDescentReader(_testFolder, _filename), std::invalid_argument);
        }

        TEST_F(GradientDescentInversionInputCardReaderTest, constructor_MissingIter_ExceptionThrown)
        {
            // Arrange
            _parameters.erase("iter");

            _jsonInput = generateJsonWithInputParameters(_parameters);
            writeInputFile(_jsonInput);

            // Act & Assert
            EXPECT_THROW(gradientDescentInversionInputCardReader gradientDescentReader(_testFolder, _filename), std::invalid_argument);
        }

    }   // namespace inversionMethods
}   // namespace fwi
