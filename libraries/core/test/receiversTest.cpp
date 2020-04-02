#include <gtest/gtest.h>
#include <iterator>
#include "receivers.h"

receivers InitializeReceivers()
{
    std::array<double, 2> xMin = {0.0, 0.0};
    std::array<double, 2> xMax = {10.0, 10.0};
    receivers receivers(xMin, xMax, 11);
    return receivers;
}

TEST(ReceiversTest, ConstructorTest)
{
    receivers receivers = InitializeReceivers();


    double dx = 1.0;
    double iteration_counter = 0.0;

    for (std::vector<std::array<double, 2>>::iterator iter = receivers.xRecv.begin(); iter != receivers.xRecv.end(); ++iter)
    {
        EXPECT_NEAR(iter->front(), iteration_counter*dx, 0.001);
        EXPECT_NEAR(iter->back(), iteration_counter*dx, 0.001);
        ++iteration_counter;
    }
}

TEST(ReceiversTest, NumberOfPositionsEqualsNumberOfReceivers)
{
    receivers receivers = InitializeReceivers();

    EXPECT_EQ(receivers.xRecv.size(), 11);
}


TEST(ReceiversTest, ReceiversAreAtExpectedPositions)
{
    receivers receivers = InitializeReceivers();

    std::array<double, 11> expectedXPositions = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0, 9.0, 10.0};

    for (unsigned int i = 0; i < expectedXPositions.size(); i++)
    {
        EXPECT_NEAR(expectedXPositions[i], receivers.xRecv[i][0], 0.001);
    }
}

