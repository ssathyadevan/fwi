#include <gtest/gtest.h>
#include "genericInputCardReader.h"
#include "genericInput.h"
#include <ostream>
#include "utilityFunctions.h"

const std::string inputPath = "./../../../../tests";

TEST(genericinputcardreadertest, readc0Test)
{
    // Given
    genericInputCardReader reader(inputPath);

    // Then
    EXPECT_NEAR(reader.getInput().c0, 2000.0, 0.0001);
}

TEST(genericinputcardreadertest, readFreqTest)
{
    // Given
    genericInputCardReader reader(inputPath);
    Freq expectedFreq = { 10, 40, 15 };
    Freq actualFreq = reader.getInput().freq;

    // Then
    EXPECT_NEAR(actualFreq.min, expectedFreq.min, 0.0001);
    EXPECT_NEAR(actualFreq.max, expectedFreq.max, 0.0001);
    EXPECT_NEAR(actualFreq.nTotal, expectedFreq.nTotal, 0.0001);
}

TEST(genericinputcardreadertest, readReservoirTest)
{
    // Given
    genericInputCardReader reader(inputPath);
    std::array<double,2> expectedTopLeft {-300.0, 0.0};
    std::array<double,2> expectedBottomRight {300.0, 300.0};

    std::array<double,2> actualTopLeft = reader.getInput().reservoirTopLeftCornerInM;
    std::array<double,2> actualBottomRight = reader.getInput().reservoirBottomRightCornerInM;


    // Then
    EXPECT_NEAR(expectedTopLeft[0], actualTopLeft[0], 0.0001);
    EXPECT_NEAR(expectedTopLeft[1], actualTopLeft[1], 0.0001);

    EXPECT_NEAR(expectedBottomRight[0], actualBottomRight[0], 0.0001);
    EXPECT_NEAR(expectedBottomRight[1], actualBottomRight[1], 0.0001);
}

TEST(genericinputcardreadertest, readSources)
{
    // Given
    genericInputCardReader reader(inputPath);

    std::array<double,2> expectedTopLeft {-480.0, -5.0};
    std::array<double,2> expectedBottomRight {480.0, -5.0};

    std::array<double,2> actualTopLeft = reader.getInput().sourcesTopLeftCornerInM;
    std::array<double,2> actualBottomRight = reader.getInput().sourcesBottomRightCornerInM;

    // Then
    EXPECT_NEAR(expectedTopLeft[0], actualTopLeft[0], 0.0001);
    EXPECT_NEAR(expectedTopLeft[1], actualTopLeft[1], 0.0001);

    EXPECT_NEAR(expectedBottomRight[0], actualBottomRight[0], 0.0001);
    EXPECT_NEAR(expectedBottomRight[1], actualBottomRight[1], 0.0001);
}


TEST(genericinputcardreadertest, readReceivers)
{
    // Given
    genericInputCardReader reader(inputPath);

    std::array<double,2> expectedTopLeft {-480.0, -5.0};
    std::array<double,2> expectedBottomRight {480.0, -5.0};

    std::array<double,2> actualTopLeft = reader.getInput().receiversTopLeftCornerInM;
    std::array<double,2> actualBottomRight = reader.getInput().receiversBottomRightCornerInM;

    // Then
    EXPECT_NEAR(expectedTopLeft[0], actualTopLeft[0], 0.0001);
    EXPECT_NEAR(expectedTopLeft[1], actualTopLeft[1], 0.0001);

    EXPECT_NEAR(expectedBottomRight[0], actualBottomRight[0], 0.0001);
    EXPECT_NEAR(expectedBottomRight[1], actualBottomRight[1], 0.0001);
}

TEST(genericinputcardreadertest, readNSourcesAndReceiversTest)
{
    // Given
    genericInputCardReader reader(inputPath);
    int expectednsources = 17;
    int expectednreceivers = 17;

    EXPECT_NEAR(reader.getInput().nSourcesReceivers.nSources, expectednsources, 0.0001);
    EXPECT_NEAR(reader.getInput().nSourcesReceivers.nReceivers, expectednreceivers, 0.0001);
}

TEST(genericinputcardreadertest, readngridTest)
{
    // Given
    genericInputCardReader reader(inputPath);
    std::array<int,2> expectedngrid {64, 32};

    // Then
    EXPECT_NEAR(reader.getInput().nGrid[0], expectedngrid[0], 0.0001);
    EXPECT_NEAR(reader.getInput().nGrid[1], expectedngrid[1], 0.0001);
}
TEST(genericinputcardreadertest, readngridoriginalTest)
{
    // Given
    genericInputCardReader reader(inputPath);
    std::array<int,2> expectedngrid {64, 32};

    // Then
    EXPECT_NEAR(reader.getInput().nGridOriginal[0], expectedngrid[0], 0.0001);
    EXPECT_NEAR(reader.getInput().nGridOriginal[1], expectedngrid[1], 0.0001);
}


TEST(genericinputcardreadertest, readVerbosityTest)
{
    // Given
    genericInputCardReader reader(inputPath);
    bool expectedVerbosity = false;

    EXPECT_EQ(expectedVerbosity, reader.getInput().verbose);
}

TEST(genericinputcardreadertest, readFileNameTest)
{
    genericInputCardReader reader(inputPath);
    std::string expectedFileName = "temple";
    EXPECT_EQ(expectedFileName, reader.getInput().fileName);
}

