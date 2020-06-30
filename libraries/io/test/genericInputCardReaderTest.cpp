#include "genericInputCardReader.h"
#include "genericInput.h"
#include "utilityFunctions.h"
#include <gtest/gtest.h>
#include <ostream>

namespace fwi
{
    namespace io
    {
        const std::string inputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        TEST(genericInputCardReaderTest, readc0Test)
        {
            // Given
            genericInputCardReader reader(inputPath);

            // Then
            EXPECT_NEAR(reader.getInput().c0, 2000.0, 0.0001);
        }

        TEST(genericInputCardReaderTest, readFreqTest)
        {
            // Given
            genericInputCardReader reader(inputPath);
            core::freqInfo expectedFreq = {10, 40, 15};
            core::freqInfo actualFreq = reader.getInput().freq;

            // Then
            EXPECT_NEAR(actualFreq.min, expectedFreq.min, 0.0001);
            EXPECT_NEAR(actualFreq.max, expectedFreq.max, 0.0001);
            EXPECT_NEAR(actualFreq.nTotal, expectedFreq.nTotal, 0.0001);
        }

        TEST(genericInputCardReaderTest, readReservoirTest)
        {
            // Given
            genericInputCardReader reader(inputPath);
            std::array<double, 2> expectedTopLeft{-300.0, 0.0};
            std::array<double, 2> expectedBottomRight{300.0, 300.0};

            std::array<double, 2> actualTopLeft = reader.getInput().reservoirTopLeftCornerInM;
            std::array<double, 2> actualBottomRight = reader.getInput().reservoirBottomRightCornerInM;

            // Then
            EXPECT_NEAR(expectedTopLeft[0], actualTopLeft[0], 0.0001);
            EXPECT_NEAR(expectedTopLeft[1], actualTopLeft[1], 0.0001);

            EXPECT_NEAR(expectedBottomRight[0], actualBottomRight[0], 0.0001);
            EXPECT_NEAR(expectedBottomRight[1], actualBottomRight[1], 0.0001);
        }

        TEST(genericInputCardReaderTest, readSources)
        {
            // Given
            genericInputCardReader reader(inputPath);

            std::array<double, 2> expectedTopLeft{-480.0, -5.0};
            std::array<double, 2> expectedBottomRight{480.0, -5.0};

            std::array<double, 2> actualTopLeft = reader.getInput().sourcesTopLeftCornerInM;
            std::array<double, 2> actualBottomRight = reader.getInput().sourcesBottomRightCornerInM;

            // Then
            EXPECT_NEAR(expectedTopLeft[0], actualTopLeft[0], 0.0001);
            EXPECT_NEAR(expectedTopLeft[1], actualTopLeft[1], 0.0001);

            EXPECT_NEAR(expectedBottomRight[0], actualBottomRight[0], 0.0001);
            EXPECT_NEAR(expectedBottomRight[1], actualBottomRight[1], 0.0001);
        }

        TEST(genericInputCardReaderTest, readReceivers)
        {
            // Given
            genericInputCardReader reader(inputPath);

            std::array<double, 2> expectedTopLeft{-480.0, -5.0};
            std::array<double, 2> expectedBottomRight{480.0, -5.0};

            std::array<double, 2> actualTopLeft = reader.getInput().receiversTopLeftCornerInM;
            std::array<double, 2> actualBottomRight = reader.getInput().receiversBottomRightCornerInM;

            // Then
            EXPECT_NEAR(expectedTopLeft[0], actualTopLeft[0], 0.0001);
            EXPECT_NEAR(expectedTopLeft[1], actualTopLeft[1], 0.0001);

            EXPECT_NEAR(expectedBottomRight[0], actualBottomRight[0], 0.0001);
            EXPECT_NEAR(expectedBottomRight[1], actualBottomRight[1], 0.0001);
        }

        TEST(genericInputCardReaderTest, readnSourcesAndReceiversTest)
        {
            // Given
            genericInputCardReader reader(inputPath);
            int expectednsources = 17;
            int expectednreceivers = 17;

            EXPECT_NEAR(reader.getInput().nSources, expectednsources, 0.0001);
            EXPECT_NEAR(reader.getInput().nReceivers, expectednreceivers, 0.0001);
        }

        TEST(genericInputCardReaderTest, readnGridTest)
        {
            // Given
            genericInputCardReader reader(inputPath);
            std::array<int, 2> expectedngrid{64, 32};

            // Then
            EXPECT_NEAR(reader.getInput().nGrid[0], expectedngrid[0], 0.0001);
            EXPECT_NEAR(reader.getInput().nGrid[1], expectedngrid[1], 0.0001);
        }

        TEST(genericInputCardReaderTest, readnGridOriginalTest)
        {
            // Given
            genericInputCardReader reader(inputPath);
            std::array<int, 2> expectedngrid{64, 32};

            // Then
            EXPECT_NEAR(reader.getInput().nGridOriginal[0], expectedngrid[0], 0.0001);
            EXPECT_NEAR(reader.getInput().nGridOriginal[1], expectedngrid[1], 0.0001);
        }

        TEST(genericInputCardReaderTest, readVerbosityTest)
        {
            // Given
            genericInputCardReader reader(inputPath);
            bool expectedVerbosity = false;

            EXPECT_EQ(expectedVerbosity, reader.getInput().verbose);
        }

        TEST(genericInputCardReaderTest, readFileNameTest)
        {
            genericInputCardReader reader(inputPath);
            std::string expectedFileName = "temple";
            EXPECT_EQ(expectedFileName, reader.getInput().fileName);
        }
    }   // namespace io
}   // namespace fwi
