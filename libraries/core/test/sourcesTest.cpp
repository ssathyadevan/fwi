#include <gtest/gtest.h>
#include <iterator>
#include "sources.h"

Sources InitializeSources()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {10.0, 10.0};
    Sources sources(xMin, xMax, 11);
    return sources;
}

TEST(SourcesTest, ConstructorTest)
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {10.0, 10.0};
    Sources sources(xMin, xMax, 11);

    double dx = 1.0;
    double iteration_counter = 0.0;

    for (std::vector<std::array<double, 2>>::iterator iter = sources.xSrc.begin(); iter != sources.xSrc.end(); ++iter)
    {
        EXPECT_NEAR(iter->front(), iteration_counter * dx, 0.001);
        EXPECT_NEAR(iter->back(), iteration_counter * dx, 0.001);
        ++iteration_counter;
    }
}

TEST(ReceiversTest, NumberOfPositionsEqualsNumberOfReceivers)
{
    Sources sources = InitializeSources();
    EXPECT_EQ(sources.xSrc.size(), 11);
}


TEST(ReceiversTest, ReceiversAreAtExpectedPositions)
{
    Sources sources = InitializeSources();

    std::array<double, 11> expectedXPositions = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

    for (unsigned int i = 0; i < expectedXPositions.size(); i++)
    {
        EXPECT_NEAR(expectedXPositions[i], sources.xSrc[i][0], 0.001);
    }
}
