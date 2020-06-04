#include "finiteDifferenceForwardModel.h"
#include "finiteDifferenceForwardModelInputCardReader.h"
#include <gtest/gtest.h>

// const std::string inputPath = "./../../../parallelized-fwi/tests"; // For local use
const std::string inputPath = "./../../../../../tests";

TEST(finiteDifferenceForwardModelInputCardReaderTest, ValidInputTest)
{
    finiteDifferenceForwardModelInputCardReader finiteDifferenceReader(inputPath);
    finiteDifferenceForwardModelInput input = finiteDifferenceReader.getInput();
    EXPECT_EQ(0.0, input.pmlWidthFactor.x);
    EXPECT_EQ(0.0, input.pmlWidthFactor.z);

    EXPECT_EQ(4, input.sourceParameter.r);
    EXPECT_EQ(6.31, input.sourceParameter.beta);
}
