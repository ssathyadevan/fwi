#include <gtest/gtest.h>
#include "genericInputCardReader.h"
#include <iostream>

//Generic Input Card test:
TEST(GenericInputTest, ints){
    std::string temp = "../../../inputFiles/test/";

    genericInputCardReader cardReader = genericInputCardReader(temp, "../FWIInstall/output/", "test");
    genericInput input = cardReader.getInput();

    EXPECT_EQ(input.ngrid[0],64);
    EXPECT_EQ(input.ngrid[1],32);
    EXPECT_EQ(input.freq.nTotal, 15);
    EXPECT_EQ(input.nSourcesReceivers.rec, 17);
    EXPECT_EQ(input.nSourcesReceivers.src, 17);

}
