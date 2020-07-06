#include "finiteDifferenceForwardModelInputCardReader.h"
#include "finiteDifferenceForwardModel.h"
#include <filesystem>
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        class finiteDifferenceForwardModelInputCardReaderTest : public ::testing::Test
        {
        protected:
            const std::string initialPath = std::string(FWI_PROJECT_DIR) + "/tests/";
            const std::string testFolder = initialPath + "FiniteDifferenceFMInputCardReaderTests/";
            const std::string inputFolder = testFolder + "input/";
            const std::string filePath = inputFolder + "FiniteDifferenceFMInput.json";

            void SetUp() override
            {
                if(!std::filesystem::exists(testFolder))
                {
                    std::filesystem::create_directory(testFolder);
                }

                if(!std::filesystem::exists(inputFolder))
                {
                    std::filesystem::create_directory(inputFolder);
                }
            }

            void TearDown() override
            {
                if(std::filesystem::exists(filePath))
                {
                    std::filesystem::remove(filePath);
                }

                if(std::filesystem::exists(inputFolder))
                {
                    std::filesystem::remove(inputFolder);
                }

                if(std::filesystem::exists(testFolder))
                {
                    std::filesystem::remove(testFolder);
                }
            }

            std::string convertInputToJsonString(
                const finiteDifferenceForwardModelInput &input, const bool addX, const bool addZ, const bool addR, const bool addBeta)
            {
                std::stringstream stream;
                stream << "{ \n";
                stream << "\t \"PMLWidthFactor\": { \n";
                if(addX)
                {
                    stream << std::fixed << std::setprecision(2) << "\t \t \"x\": " << input.pmlWidthFactor.x;
                    if(addZ)
                    {
                        stream << ", ";
                    }
                    stream << "\n ";
                }
                if(addZ)
                {
                    stream << std::fixed << std::setprecision(2) << "\t \t \"z\": " << input.pmlWidthFactor.z << " \n";
                }
                stream << "\t }, \n";
                stream << "\t \"SourceParameter\": { \n";
                if(addR)
                {
                    stream << std::fixed << std::setprecision(2) << "\t \t \"r\": " << input.sourceParameter.r;
                    if(addBeta)
                    {
                        stream << ", ";
                    }
                    stream << "\n ";
                }
                if(addBeta)
                {
                    stream << std::fixed << std::setprecision(2) << "\t \t \"beta\": " << input.sourceParameter.beta << "\n";
                }
                stream << "\t } \n";
                stream << "}";

                return stream.str();
            }

            void writeInputFile(const std::string &jsonInputString)
            {
                std::ofstream inputFile;
                inputFile.open(filePath);
                inputFile << jsonInputString << std::endl;
                inputFile.close();
            }
        };

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, constructor_ValidInput)
        {
            finiteDifferenceForwardModelInput writeInput;
            PMLWidthFactor pmlWidthFactor(0.0, 0.0);
            SourceParameter sourceParameter(4, 6.31);

            writeInput.pmlWidthFactor = pmlWidthFactor;
            writeInput.sourceParameter = sourceParameter;

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, true, true);

            writeInputFile(jsonInput);

            finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder);
            finiteDifferenceForwardModelInput input = finiteDifferenceReader.getInput();
            EXPECT_EQ(0.0, input.pmlWidthFactor.x);
            EXPECT_EQ(0.0, input.pmlWidthFactor.z);

            EXPECT_EQ(4, input.sourceParameter.r);
            EXPECT_EQ(6.31, input.sourceParameter.beta);

            EXPECT_EQ(leastSquares, input.costFunction);
            EXPECT_EQ(BoundaryConditionType::FirstOrderABC, input.boundaryConditionType);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingXVariable_ExceptionsThrown)
        {
            finiteDifferenceForwardModelInput writeInput;
            PMLWidthFactor pmlWidthFactor(0.0, 0.0);
            SourceParameter sourceParameter(999, 999);

            writeInput.pmlWidthFactor = pmlWidthFactor;
            writeInput.sourceParameter = sourceParameter;

            std::string jsonInput = convertInputToJsonString(writeInput, false, true, true, true);
            writeInputFile(jsonInput);

            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }
        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingZVariable_ExceptionsThrown)
        {
            finiteDifferenceForwardModelInput writeInput;
            PMLWidthFactor pmlWidthFactor(0.0, 0.0);
            SourceParameter sourceParameter(999, 999);

            writeInput.pmlWidthFactor = pmlWidthFactor;
            writeInput.sourceParameter = sourceParameter;

            std::string jsonInput = convertInputToJsonString(writeInput, true, false, true, true);
            writeInputFile(jsonInput);

            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingRVariable_ExceptionsThrown)
        {
            finiteDifferenceForwardModelInput writeInput;
            PMLWidthFactor pmlWidthFactor(0.0, 0.0);
            SourceParameter sourceParameter(999, 999);

            writeInput.pmlWidthFactor = pmlWidthFactor;
            writeInput.sourceParameter = sourceParameter;

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, false, true);
            writeInputFile(jsonInput);

            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }

        TEST_F(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingBetaVariable_ExceptionsThrown)
        {
            finiteDifferenceForwardModelInput writeInput;
            PMLWidthFactor pmlWidthFactor(0.0, 0.0);
            SourceParameter sourceParameter(999, 999);

            writeInput.pmlWidthFactor = pmlWidthFactor;
            writeInput.sourceParameter = sourceParameter;

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, true, false);
            writeInputFile(jsonInput);

            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);
        }
    }   // namespace forwardModels
}   // namespace fwi
