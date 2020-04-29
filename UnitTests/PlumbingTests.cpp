#include "gtest/gtest.h"
#include "utils.h"

namespace PlumbingTests
{
    TEST(UtilsTest, SplitAndJoinTest)
    {
        uint16_t data = 0x3537;
        EXPECT_EQ(Bit16Join(Bit16Split(data)), data);
    }

    TEST(PlumbingTest, Bit1SelectorTest)
    {
        //                     S d0 d1
        EXPECT_EQ(Bit1Selector(0, 0, 0), 0);
        EXPECT_EQ(Bit1Selector(1, 0, 0), 0);
        EXPECT_EQ(Bit1Selector(0, 0, 1), 0);
        EXPECT_EQ(Bit1Selector(1, 0, 1), 1);
        EXPECT_EQ(Bit1Selector(0, 1, 0), 1);
        EXPECT_EQ(Bit1Selector(1, 1, 0), 0);
        EXPECT_EQ(Bit1Selector(0, 1, 1), 1);
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

    TEST(PlumbingTest, Bit16SwitchTest)
    {
        uint16_t data0 = 0x3537;
        uint16_t data1 = 0x1337;
        EXPECT_EQ(Bit16Selector(0, data0, data1), data0);
        EXPECT_EQ(Bit16Selector(1, data0, data1), data1);
    }
}
