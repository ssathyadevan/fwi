#include "conjugateGradientInversionInputCardReader.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace inversionMethods
    {
        class ConjugateGradientInversionInputCardReaderTest : public ::testing::Test
        {
        protected:
            using Parameters = std::map<std::string, std::string>;
            using ParametersCollection = std::map<std::string, Parameters>;

            const std::string _testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string _inputFolder = _testFolder + "input/";
            const std::string _filename = "ConjugateGradientInversionInputTest.json";
            const std::string _filePath = _inputFolder + _filename;

            ParametersCollection _groupParameters{
                {"Iter1", {{"n", "10"}, {"tolerance", "1e-08"}}}, {"DeltaAmplification", {{"start", "100.0"}, {"slope", "10.0"}}}};

            Parameters _singleParameters{{"n_max", "5"}, {"do_reg", "true"}};

            std::string _jsonInput;

            void SetUp() override { _jsonInput = generateJsonWithInputParameters(_groupParameters, _singleParameters); };

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

        TEST_F(ConjugateGradientInversionInputCardReaderTest, constructor_ValidInput_Iter1)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            ConjugateGradientInversionInputCardReader conjugateGradientReader(_testFolder, _filename);
            auto input = conjugateGradientReader.getInput();

            // Assert
            EXPECT_EQ(10, input.iteration1.n);
            ASSERT_DOUBLE_EQ(1e-8, input.iteration1.tolerance);
        }

        TEST_F(ConjugateGradientInversionInputCardReaderTest, constructor_ValidInput_dAmplification)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            ConjugateGradientInversionInputCardReader conjugateGradientReader(_testFolder, _filename);
            auto input = conjugateGradientReader.getInput();

            // Assert
            ASSERT_DOUBLE_EQ(100, input.dAmplification.start);
            ASSERT_DOUBLE_EQ(10, input.dAmplification.slope);
        }

        TEST_F(ConjugateGradientInversionInputCardReaderTest, constructor_ValidInput_Nmax)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            ConjugateGradientInversionInputCardReader conjugateGradientReader(_testFolder, _filename);
            auto input = conjugateGradientReader.getInput();

            // Assert
            EXPECT_EQ(5, input.n_max);
        }

        TEST_F(ConjugateGradientInversionInputCardReaderTest, constructor_ValidInput_doRegularisation)
        {
            // Arrange
            writeInputFile(_jsonInput);

            // Act
            ConjugateGradientInversionInputCardReader conjugateGradientReader(_testFolder, _filename);
            auto input = conjugateGradientReader.getInput();

            // Assert
            EXPECT_EQ(true, input.doRegularisation);
        }
    }   // namespace inversionMethods
}   // namespace fwi
