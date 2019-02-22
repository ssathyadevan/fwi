#include <gtest/gtest.h>
#include "genericInputCardReader.h"

TEST(GenericInputTest, ints){
    genericInputCardReader cardReader = genericInputCardReader("/input/","/output","testGenericInput");
    genericInput input = cardReader.getInput();

}
