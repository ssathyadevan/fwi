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

            const std::string initialPath = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string testFolder = initialPath + "FiniteDifferenceFMInputCardReaderTests/";
            const std::string inputFolder = testFolder + "input/";
            const std::string filePath = inputFolder + "FiniteDifferenceFMInput.json";

            void SetUp() override
            {
                struct stat stats;
                stat((testFolder).c_str(), &stats);
                if(!S_ISDIR(stats.st_mode))
                {
#if __unix__
                    mkdir((testFolder).c_str(), 0777);
#else
                    mkdir((testFolder).c_str());
#endif
                }
                stat((inputFolder).c_str(), &stats);
                if(!S_ISDIR(stats.st_mode))
                {
#if __unix__
                    mkdir((inputFolder).c_str(), 0777);
#else
                    mkdir((inputFolder).c_str());
#endif
                }
            }
            void TearDown() override
            {
                if(remove((filePath).c_str()) != 0)
                {
                    perror("Error deleting finiteDifferenceFMInput file");
                }
                if(rmdir((inputFolder).c_str()) != 0)
                {
                    perror("Error deleting input directory");
                }
                if(rmdir((testFolder).c_str()) != 0)
                {
                    perror("Error deleting testInputFiles directory");
                }
            }

            std::string generateJsonWithInputParameters(const Parameters &pmlWidth, const Parameters &source)
            {
                std::stringstream s;
                s << "{\n";
                addToJson("PMLWidthFactor", pmlWidth, s);
                s << ",\n";
                addToJson("SourceParameter", source, s);
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

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_ValidInput)
        {
            // Arrange
            Parameters pmlWidth{{"x", "0.0"}, {"z", "0.0"}};
            Parameters sources{{"r", "4"}, {"beta", "6.31"}};

            auto jsonInput = generateJsonWithInputParameters(pmlWidth, sources);
            writeInputFile(jsonInput);

            // Act
            finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder);
            finiteDifferenceForwardModelInput input = finiteDifferenceReader.getInput();

            // Assert
            ASSERT_DOUBLE_EQ(0.0, input.pmlWidthFactor.x);
            ASSERT_DOUBLE_EQ(0.0, input.pmlWidthFactor.z);

            EXPECT_EQ(4, input.sourceParameter.r);
            EXPECT_EQ(6.31, input.sourceParameter.beta);

            EXPECT_EQ(leastSquares, input.costFunction);
            EXPECT_EQ(BoundaryConditionType::FirstOrderABC, input.boundaryConditionType);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingXVariable_ExceptionsThrown)
        {
            // Arrange
            Parameters pmlWidthWithoutX{{"z", "0.0"}};
            Parameters sources{{"r", "4"}, {"beta", "6.31"}};
            auto jsonInput = generateJsonWithInputParameters(pmlWidthWithoutX, sources);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingZVariable_ExceptionsThrown)
        {
            // Arrange
            Parameters pmlWidthWithoutZ{{"x", "1.0"}};
            Parameters sources{{"r", "999"}, {"beta", "999"}};

            auto jsonInput = generateJsonWithInputParameters(pmlWidthWithoutZ, sources);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingRVariable_ExceptionsThrown)
        {
            // Arrange
            Parameters pmlWidth{{"x", "1.0"}, {"z", "3.4"}};
            Parameters sourcesWithoutR{{"beta", "999"}};

            auto jsonInput = generateJsonWithInputParameters(pmlWidth, sourcesWithoutR);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingBetaVariable_ExceptionsThrown)
        {
            // Arrange
            Parameters pmlWidth{{"x", "1.0"}, {"z", "3.4"}};
            Parameters sourcesWithoutBeta{{"r", "999"}};

            auto jsonInput = generateJsonWithInputParameters(pmlWidth, sourcesWithoutBeta);
            writeInputFile(jsonInput);

            // Act & Assert
            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }
    }   // namespace forwardModels
}   // namespace fwi
