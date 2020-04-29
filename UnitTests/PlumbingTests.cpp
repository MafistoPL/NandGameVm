#include "gtest/gtest.h"
#include "utils.h"

namespace PlumbingTests
{
    TEST(PlumbingTest, Bit1SelectorTest)
    {
        //                     X  Y  S
        EXPECT_EQ(Bit1Selector(0, 0, 0), 0);
        EXPECT_EQ(Bit1Selector(0, 0, 1), 0);
        EXPECT_EQ(Bit1Selector(0, 1, 0), 0);
        EXPECT_EQ(Bit1Selector(0, 1, 1), 1);
        EXPECT_EQ(Bit1Selector(1, 0, 0), 1);
        EXPECT_EQ(Bit1Selector(1, 0, 1), 0);
        EXPECT_EQ(Bit1Selector(1, 1, 0), 1);
        EXPECT_EQ(Bit1Selector(1, 1, 1), 1);
    }

    TEST(PlumbingTest, Bit1SwitchTest)
    {
        bool c0, c1;
        
        Bit1Switch(0, 0, &c0, &c1);
        EXPECT_EQ(c0, 0);
        EXPECT_EQ(c1, 0);

        Bit1Switch(0, 1, &c0, &c1);
        EXPECT_EQ(c0, 1);
        EXPECT_EQ(c1, 0);

        Bit1Switch(1, 0, &c0, &c1);
        EXPECT_EQ(c0, 0);
        EXPECT_EQ(c1, 0);

        Bit1Switch(1, 1, &c0, &c1);
        EXPECT_EQ(c0, 0);
        EXPECT_EQ(c1, 1);
    }
}
