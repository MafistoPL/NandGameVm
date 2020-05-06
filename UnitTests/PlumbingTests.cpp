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

    TEST(PlumbingTest, Bit16SelectorTest)
    {
        EXPECT_EQ(Bit16Selector(0, 0, 0), 0);
        EXPECT_EQ(Bit16Selector(1, 0, 0), 0);
        EXPECT_EQ(Bit16Selector(0, 0, 0x1234), 0);
        EXPECT_EQ(Bit16Selector(1, 0, 0x1234), 0x1234);
        EXPECT_EQ(Bit16Selector(0, 0x9876, 0), 0x9876);
        EXPECT_EQ(Bit16Selector(1, 0x9876, 0), 0);
        EXPECT_EQ(Bit16Selector(0, 0xAAAA, 0x5555), 0xAAAA);
        EXPECT_EQ(Bit16Selector(1, 0xAAAA, 0x5555), 0x5555);
    }

    TEST(PlumbingTest, Bit16Selector4WayTest)
    {
        std::vector<uint16_t> emptyData(4);
        std::vector<uint16_t> data = { 0x4321, 0x9876, 0xAAAA, 0x5555 };
        std::vector<bool> dataSwitch = { 0, 0 };

        dataSwitch[0] = 0;
        dataSwitch[1] = 0;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 1;
        dataSwitch[1] = 0;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 0;
        dataSwitch[1] = 1;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 1;
        dataSwitch[1] = 1;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, emptyData), 0);

        dataSwitch[0] = 0;
        dataSwitch[1] = 0;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, data), 0x4321);
        dataSwitch[0] = 1;
        dataSwitch[1] = 0;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, data), 0x9876);
        dataSwitch[0] = 0;
        dataSwitch[1] = 1;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, data), 0xAAAA);
        dataSwitch[0] = 1;
        dataSwitch[1] = 1;
        EXPECT_EQ(Bit16Selector4Way(dataSwitch, data), 0x5555);
    }

    TEST(PlumbingTest, Bit16Selector8WayTest)
    {
        std::vector<uint16_t> emptyData(8);
        std::vector<uint16_t> data = { 0x4321, 0x5432, 0x6543, 0x7654, 0x8765, 0x9876, 0xA987, 0xBA98 };
        std::vector<bool> dataSwitch = { 0, 0, 0 };

        dataSwitch[0] = 0;
        dataSwitch[1] = 0;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 1;
        dataSwitch[1] = 0;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 0;
        dataSwitch[1] = 1;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 1;
        dataSwitch[1] = 1;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 0;
        dataSwitch[1] = 0;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 1;
        dataSwitch[1] = 0;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 0;
        dataSwitch[1] = 1;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);
        dataSwitch[0] = 1;
        dataSwitch[1] = 1;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, emptyData), 0);

        dataSwitch[0] = 0;
        dataSwitch[1] = 0;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data),0x4321);
        dataSwitch[0] = 1;
        dataSwitch[1] = 0;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data), 0x5432);
        dataSwitch[0] = 0;
        dataSwitch[1] = 1;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data), 0x6543);
        dataSwitch[0] = 1;
        dataSwitch[1] = 1;
        dataSwitch[2] = 0;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data), 0x7654);
        dataSwitch[0] = 0;
        dataSwitch[1] = 0;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data), 0x8765);
        dataSwitch[0] = 1;
        dataSwitch[1] = 0;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data), 0x9876);
        dataSwitch[0] = 0;
        dataSwitch[1] = 1;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data), 0xA987);
        dataSwitch[0] = 1;
        dataSwitch[1] = 1;
        dataSwitch[2] = 1;
        EXPECT_EQ(Bit16Selector8Way(dataSwitch, data), 0xBA98);
    }

    TEST(PlumbingTest, Bit1SwitchTest)
    {
        std::vector<bool> output;
        
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

    TEST(PlumbingTest, Bit1Switch4WayTest)
    {
        std::vector<bool> output;
        std::vector<bool> canalSwitch = { 0, 0 };

        canalSwitch[0] = 0;
        canalSwitch[1] = 0;
        Bit1Switch4Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 0;
        Bit1Switch4Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 1;
        Bit1Switch4Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 1;
        Bit1Switch4Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 0;
        Bit1Switch4Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 1);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 0;
        Bit1Switch4Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 1);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 1;
        Bit1Switch4Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 1);
        EXPECT_EQ(output[3], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 1;
        Bit1Switch4Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 1);
    }
    TEST(PlumbingTest, Bit1Switch8WayTest)
    {
        std::vector<bool> output;
        std::vector<bool> canalSwitch = { 0, 0, 0 };

        canalSwitch[0] = 0;
        canalSwitch[1] = 0;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 0;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 1;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 1;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 0;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 0;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 1;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 1;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 0, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 0;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 1);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 0;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 1);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 1;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 1);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 1;
        canalSwitch[2] = 0;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 1);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 0;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 1);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 0;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 1);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 0;
        canalSwitch[1] = 1;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 1);
        EXPECT_EQ(output[7], 0);

        canalSwitch[0] = 1;
        canalSwitch[1] = 1;
        canalSwitch[2] = 1;
        Bit1Switch8Way(canalSwitch, 1, output);
        EXPECT_EQ(output[0], 0);
        EXPECT_EQ(output[1], 0);
        EXPECT_EQ(output[2], 0);
        EXPECT_EQ(output[3], 0);
        EXPECT_EQ(output[4], 0);
        EXPECT_EQ(output[5], 0);
        EXPECT_EQ(output[6], 0);
        EXPECT_EQ(output[7], 1);
    }

    TEST(PlumbingTest, Bit16SwitchTest)
    {
        uint16_t data0 = 0x3537;
        uint16_t data1 = 0x1337;
        EXPECT_EQ(Bit16Selector(0, data0, data1), data0);
        EXPECT_EQ(Bit16Selector(1, data0, data1), data1);
    }
}
