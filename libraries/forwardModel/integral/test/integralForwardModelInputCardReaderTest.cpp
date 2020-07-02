#include "integralForwardModelInputCardReader.h"
#include "integralForwardModel.h"
#include <gtest/gtest.h>

namespace fwi
{
    namespace forwardModels
    {
        const std::string initialPath = std::string(FWI_PROJECT_DIR) + "/tests";
        const std::string testFolder = initialPath + "/integralFMInputCardReaderTests/";
        const std::string inputFolder = testFolder + "input/";
        const std::string filePath = inputFolder + "IntegralFMInput.json";

        std::string convertInputToJsonString(const integralForwardModelInput &input, const bool addN, const bool addTolerance, const bool addCalcAlpha)
        {
            std::stringstream stream;
            stream << "{ \n";
            stream << "\t \"Iter2\": { \n";
            if(addN)
            {
                stream << std::fixed << std::setprecision(2) << "\t \t \"n\": " << input.nrOfIterations;
                if(addTolerance || addCalcAlpha)
                {
                    stream << ", ";
                }
                stream << "\n ";
            }
            if(addTolerance)
            {
                stream << std::fixed << std::setprecision(1) << "\t \t \"tolerance\": " << input.tolerance;
                if(addCalcAlpha)
                {
                    stream << ", ";
                }
                stream << "\n ";
            }
            if(addCalcAlpha)
            {
                stream << std::fixed << std::setprecision(2) << "\t \t \"calcAlpha\": " << std::boolalpha << input.calcAlpha << "\n";
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
                perror("Error deleting integralFMInput file");
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

        TEST(integralForwardModelInputCardReaderTest, constructor_ValidInput)
        {
            integralForwardModelInput writeInput(1, 1.2, false);

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, true);

            writeInputFile(jsonInput);
            integralForwardModelInputCardReader integralForwardModelReader(testFolder);

            integralForwardModelInput input = integralForwardModelReader.getInput();

            EXPECT_EQ(1, input.nrOfIterations);
            ASSERT_DOUBLE_EQ(1.2, input.tolerance);
            ASSERT_FALSE(input.calcAlpha);
            EXPECT_EQ(leastSquares, input.costFunction);
        }

        TEST(integralForwardModelInputCardReaderTest, constructor_negativeNVariable_ExceptionThrown)
        {
            integralForwardModelInput writeInput(0, 1.2, false);

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, true);

            writeInputFile(jsonInput);

            EXPECT_THROW(integralForwardModelInputCardReader integralForwardModelReader(testFolder), std::invalid_argument);
        }
        TEST(integralForwardModelInputCardReaderTest, constructor_negativeToleranceVariable_ExceptionThrown)
        {
            integralForwardModelInput writeInput(1, -1.0, false);

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, true);

            writeInputFile(jsonInput);

            EXPECT_THROW(integralForwardModelInputCardReader integralForwardModelReader(testFolder), std::invalid_argument);
        }

        TEST(integralForwardModelInputCardReaderTest, constructor_missingNVariable_ExceptionThrown)
        {
            integralForwardModelInput writeInput(1, 1.2, false);

            std::string jsonInput = convertInputToJsonString(writeInput, false, true, true);

            writeInputFile(jsonInput);

            EXPECT_THROW(integralForwardModelInputCardReader integralForwardModelReader(testFolder), std::invalid_argument);
        }

        TEST(integralForwardModelInputCardReaderTest, constructor_missingToleranceVariable_ExceptionThrown)
        {
            integralForwardModelInput writeInput(1, 1.2, false);

            std::string jsonInput = convertInputToJsonString(writeInput, true, false, true);

            writeInputFile(jsonInput);

            EXPECT_THROW(integralForwardModelInputCardReader integralForwardModelReader(testFolder), std::invalid_argument);
        }
        TEST(integralForwardModelInputCardReaderTest, constructor_missingCalcAlphaVariable_ExceptionThrown)
        {
            integralForwardModelInput writeInput(1, 1.2, false);

            std::string jsonInput = convertInputToJsonString(writeInput, true, true, false);

            writeInputFile(jsonInput);

            EXPECT_THROW(integralForwardModelInputCardReader integralForwardModelReader(testFolder), std::invalid_argument);

            // removeInputFile();
        }

    }   // namespace forwardModels
}   // namespace fwi
