#include "StepAndDirectionReconstructorInputCardReader.h"
#include <gtest/gtest.h>
#include <sys/stat.h>

const std::string inputPath = "./../../../parallelized-fwi/tests";   // For local use
// const std::string inputPath = "./../../../../tests";

const std::string writePath = "./../";
const std::string testFolder = writePath + "/testInputFiles";

std::string convertInputToJsonString(const StepAndDirectionReconstructorInput &input)
{
    std::stringstream stream;
    stream << "{ \n";
    stream << "\t \"ReconstructorParameters\": { \n";
    stream << std::fixed << std::setprecision(2) << "\t \t \"Tolerance\": " << input.reconstructorParameters.tolerance << ", \n ";
    stream << std::fixed << std::setprecision(2) << "\t \t \"InitialChi\": " << input.reconstructorParameters.startingChi << ", \n";
    stream << " \t \t \"MaxIterationNumber\": " << input.reconstructorParameters.maxIterationsNumber << "\n";
    stream << "\t }, \n";
    stream << "\t \"StepSizeParameters\": { \n";
    stream << std::fixed << std::setprecision(2) << "\t \t \"InitialStepSize\": " << input.stepSizeParameters.initialStepSize << ", \n";
    stream << std::fixed << std::setprecision(2) << "\t \t \"Slope\": " << input.stepSizeParameters.slope << "\n";
    stream << "\t }, \n";
    stream << "\t \"DirectionParameters\": { \n";
    stream << std::fixed << std::setprecision(2) << "\t \t \"DerivativeStepSize\": " << input.directionParameters.derivativeStepSize << "\n";
    stream << "\t }, \n";
    stream << std::boolalpha << "\t \"DoRegularisation\": " << input.doConjugateGradientRegularisation << "\n";
    stream << "}";

    return stream.str();
}

void writeInputFile(const std::string &jsonInputString)
{
    struct stat stats;
    stat((writePath + "/testInputFiles").c_str(), &stats);
    if(!S_ISDIR(stats.st_mode))
    {
        mkdir((writePath + "/testInputFiles").c_str(), 0777);
        mkdir((writePath + "/testInputFiles/input").c_str(), 0777);
    }
    std::ofstream inputFile;
    inputFile.open(writePath + "/testInputFiles/input/StepAndDirectionInput.json");
    inputFile << jsonInputString << std::endl;
    inputFile.close();
}

void removeInputFile()
{
    if(remove((writePath + "/testInputFiles/input/StepAndDirectionInput.json").c_str()) != 0)
    {
        perror("Error deleting StepAndDirectionInput file");
    }
    if(rmdir((writePath + "/testInputFiles/input").c_str()) != 0)
    {
        perror("Error deleting input directory");
    }
    if(rmdir((writePath + "/testInputFiles").c_str()) != 0)
    {
        perror("Error deleting testInputFiles directory");
    }
}

TEST(StepAndDirectionReconstructorInputCardReaderTest, writeFileTest)
{
    StepAndDirectionReconstructorInput writeInput;
    ReconstructorParameters reconstructParameters(0.01, 0.5, 10);
    StepSizeParameters stepSizeParameters(1.0, -0.01);
    DirectionParameters directionParameters(1.0);

    writeInput.reconstructorParameters = reconstructParameters;
    writeInput.stepSizeParameters = stepSizeParameters;
    writeInput.directionParameters = directionParameters;
    writeInput.doConjugateGradientRegularisation = true;

    std::string jsonInput = convertInputToJsonString(writeInput);
    writeInputFile(jsonInput);

    StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(testFolder);

    StepAndDirectionReconstructorInput readInput = stepAndDirectionReader.getInput();

    ASSERT_EQ(readInput.reconstructorParameters.maxIterationsNumber, writeInput.reconstructorParameters.maxIterationsNumber);
    ASSERT_EQ(readInput.reconstructorParameters.startingChi, writeInput.reconstructorParameters.startingChi);
    ASSERT_EQ(readInput.reconstructorParameters.tolerance, writeInput.reconstructorParameters.tolerance);

    ASSERT_EQ(readInput.stepSizeParameters.initialStepSize, writeInput.stepSizeParameters.initialStepSize);
    ASSERT_EQ(readInput.stepSizeParameters.slope, writeInput.stepSizeParameters.slope);

    ASSERT_EQ(readInput.directionParameters.derivativeStepSize, writeInput.directionParameters.derivativeStepSize);

    ASSERT_TRUE(readInput.doConjugateGradientRegularisation == writeInput.doConjugateGradientRegularisation);

    removeInputFile();
}
