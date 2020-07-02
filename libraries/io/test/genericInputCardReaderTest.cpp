#include "genericInputCardReader.h"
#include "genericInput.h"
#include "utilityFunctions.h"
#include <gtest/gtest.h>
#include <ostream>

namespace fwi
{
    namespace io
    {
        const std::string validInputPath = std::string(FWI_PROJECT_DIR) + "/tests";

        TEST(genericInputCardReaderTest, readc0Test)
        {
            // Given
            genericInputCardReader reader(validInputPath);

            // Then
            EXPECT_NEAR(reader.getInput().c0, 2000.0, 0.0001);
        }

        TEST(genericInputCardReaderTest, readFreqTest)
        {
            // Given
            genericInputCardReader reader(validInputPath);
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
            genericInputCardReader reader(validInputPath);
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
            genericInputCardReader reader(validInputPath);

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
            genericInputCardReader reader(validInputPath);

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
            genericInputCardReader reader(validInputPath);
            int expectednsources = 17;
            int expectednreceivers = 17;

            EXPECT_NEAR(reader.getInput().nSources, expectednsources, 0.0001);
            EXPECT_NEAR(reader.getInput().nReceivers, expectednreceivers, 0.0001);
        }

        TEST(genericInputCardReaderTest, readnGridTest)
        {
            // Given
            genericInputCardReader reader(validInputPath);
            std::array<int, 2> expectedngrid{64, 32};

            // Then
            EXPECT_NEAR(reader.getInput().nGrid[0], expectedngrid[0], 0.0001);
            EXPECT_NEAR(reader.getInput().nGrid[1], expectedngrid[1], 0.0001);
        }

        TEST(genericInputCardReaderTest, readnGridOriginalTest)
        {
            // Given
            genericInputCardReader reader(validInputPath);
            std::array<int, 2> expectedngrid{64, 32};

            // Then
            EXPECT_NEAR(reader.getInput().nGridOriginal[0], expectedngrid[0], 0.0001);
            EXPECT_NEAR(reader.getInput().nGridOriginal[1], expectedngrid[1], 0.0001);
        }

        TEST(genericInputCardReaderTest, readVerbosityTest)
        {
            // Given
            genericInputCardReader reader(validInputPath);
            bool expectedVerbosity = false;

            EXPECT_EQ(expectedVerbosity, reader.getInput().verbose);
        }

        TEST(genericInputCardReaderTest, readFileNameTest)
        {
            genericInputCardReader reader(validInputPath);
            std::string expectedFileName = "temple";
            EXPECT_EQ(expectedFileName, reader.getInput().fileName);
        }

        const std::string invalidInputPath = std::string(FWI_PROJECT_DIR) + "/tests/invalidInput";
        TEST(genericInputCardReaderTest, invalidInputTest)
        {
            // this only tests that nSources cannot be <=1
            EXPECT_THROW(genericInputCardReader reader(invalidInputPath), std::invalid_argument);
        }

    }   // namespace io
}   // namespace fwi
