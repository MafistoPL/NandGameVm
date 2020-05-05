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
        std::vector<bool> output(2);
        
        Bit1Switch(0, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);

        Bit1Switch(0, 1, output);
        EXPECT_EQ(output[0], 1);
        EXPECT_EQ(output[1], 0);

        Bit1Switch(1, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);

        Bit1Switch(1, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 1);
    }

    TEST(PlumbingTest, Bit16SwitchTest)
    {
        uint16_t data0 = 0x3537;
        uint16_t data1 = 0x1337;
        EXPECT_EQ(Bit16Selector(0, data0, data1), data0);
        EXPECT_EQ(Bit16Selector(1, data0, data1), data1);
    }
}
