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

            const std::string testFolder = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string inputFolder = testFolder + "input/";
            const std::string filename = "IntegralFMInputTest.json";
            const std::string filePath = inputFolder + filename;

            void SetUp() override {}
            void TearDown() override
            {
                if(remove((filePath).c_str()) != 0)
                {
                    perror("Error deleting integralFMInput file");
                }
            }

            std::string generateJsonWithInputParameters(const Parameters &iter2)
            {
                std::stringstream s;
                s << "{\n";
                addToJson("Iter2", iter2, s);
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

        TEST_F(integralForwardModelInputCardReaderTest, constructor_ValidInput)
        {
            // Arrange
            Parameters iter2{{"n", "15"}, {"tolerance", "5.0e-5"}, {"calcAlpha", "false"}};

            auto jsonInput = generateJsonWithInputParameters(iter2);
            writeInputFile(jsonInput);

            // Act
            integralForwardModelInputCardReader integralReader(testFolder, filename);
            integralForwardModelInput input = integralReader.getInput();

            // Assert
            EXPECT_EQ(15, input.nrOfIterations);
            ASSERT_DOUBLE_EQ(5.0e-5, input.tolerance);
            ASSERT_FALSE(input.calcAlpha);
            EXPECT_EQ(leastSquares, input.costFunction);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructorInvalidNValue_ExceptionThrown)
        {
            // Arrange
            Parameters iter2{{"n", "0"}, {"tolerance", "5.0e-5"}, {"calcAlpha", "false"}};

            auto jsonInput = generateJsonWithInputParameters(iter2);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_invalidToleranceValue_ExceptionThrown)
        {
            // Arrange
            Parameters iter2{{"n", "5"}, {"tolerance", "-1.0"}, {"calcAlpha", "false"}};

            auto jsonInput = generateJsonWithInputParameters(iter2);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_missingNVariable_ExceptionThrown)
        {
            // Arrange
            Parameters iter2{{"tolerance", "5.0e-5"}, {"calcAlpha", "false"}};

            auto jsonInput = generateJsonWithInputParameters(iter2);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_missingToleranceVariable_ExceptionThrown)
        {
            // Arrange
            Parameters iter2{{"n", "5"}, {"calcAlpha", "false"}};

            auto jsonInput = generateJsonWithInputParameters(iter2);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(testFolder, filename), std::invalid_argument);
        }

        TEST_F(integralForwardModelInputCardReaderTest, constructor_missingCalcAlphaVariable_ExceptionThrown)
        {
            // Arrange
            Parameters iter2{{"n", "5"}, {"tolerance", "5.0e-5"}};

            auto jsonInput = generateJsonWithInputParameters(iter2);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(integralForwardModelInputCardReader integralReader(testFolder, filename), std::invalid_argument);
        }

    }   // namespace forwardModels
}   // namespace fwi
