#include "finiteDifferenceForwardModelInputCardReader.h"
#include "finiteDifferenceForwardModel.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        //    const std::string writePath = "./../";
        //    const std::string testFolder = writePath + "testInputFiles/";
        //    const std::string inputFolder = testFolder + "input/";
        //    const std::string filePath = inputFolder + "StepAndDirectionInput.json";

        const std::string initialPath = std::string(FWI_PROJECT_DIR) + "/tests/";
        const std::string testFolder = initialPath + "FiniteDifferenceFMInputCardReaderTests/";
        const std::string inputFolder = testFolder + "input/";
        const std::string filePath = inputFolder + "FiniteDifferenceFMInput.json";

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
            struct stat stats;
            stat((testFolder).c_str(), &stats);
            if(!S_ISDIR(stats.st_mode))
            {
                mkdir((testFolder).c_str(), 0777);
                mkdir((inputFolder).c_str(), 0777);
            }
            std::ofstream inputFile;
            inputFile.open(filePath);
            inputFile << jsonInputString << std::endl;
            inputFile.close();
        }

        void removeInputFile()
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

        TEST(finiteDifferenceForwardModelInputCardReaderTest, constructor_ValidInput)
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

        TEST(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingXVariable_ExceptionsThrown)
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
        TEST(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingZVariable_ExceptionsThrown)
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

        TEST(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingRVariable_ExceptionsThrown)
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

        TEST(finiteDifferenceForwardModelInputCardReaderTest, construtor_missingBetaVariable_ExceptionsThrown)
        {
            finiteDifferenceForwardModelInput writeInput;
            PMLWidthFactor pmlWidthFactor(0.0, 0.0);
            SourceParameter sourceParameter(999, 999);

            writeInput.pmlWidthFactor = pmlWidthFactor;
            writeInput.sourceParameter = sourceParameter;

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, true, false);
            writeInputFile(jsonInput);

            EXPECT_THROW(finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(testFolder), std::invalid_argument);

            removeInputFile();
        }
    }   // namespace forwardModels
}   // namespace fwi
