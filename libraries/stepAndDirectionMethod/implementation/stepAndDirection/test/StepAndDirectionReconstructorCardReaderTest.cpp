#include "StepAndDirectionReconstructorInputCardReader.h"
#include <gtest/gtest.h>

TEST(StepAndDirectionReconstructorInputCardReaderTest, readTest)
{
    const std::string caseFolder = "";
    StepAndDirectionReconstructorInputCardReader stepAndDirectionReader(caseFolder);

    StepAndDirectionReconstructorInput input = stepAndDirectionReader.getInput();

    ASSERT_EQ(input.reconstructorParameters.maxIterationsNumber, 10);
    ASSERT_EQ(input.reconstructorParameters.startingChi, 0.5);
    ASSERT_EQ(input.reconstructorParameters.tolerance, 0.01);

    ASSERT_EQ(input.stepSizeParameters.initialStepSize, 1.0);
    ASSERT_EQ(input.stepSizeParameters.slope, -0.01);

    ASSERT_EQ(input.directionParameters.derivativeStepSize, 1.0);

    ASSERT_FALSE(input.doConjugateGradientRegularisation);
}
