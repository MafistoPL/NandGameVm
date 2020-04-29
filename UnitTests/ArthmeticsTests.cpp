#include "gtest/gtest.h"
#include "arthmetics.h"

namespace ArthmeticsTests
{
    TEST(AdderTest, HalfAdderTest)
    {
        OneBitAdderResult r = HalfAdder(0, 0);
        EXPECT_EQ(r.l, 0);
        EXPECT_EQ(r.h, 0);

        r = HalfAdder(0, 1);
        EXPECT_EQ(r.l, 1);
        EXPECT_EQ(r.h, 0);

        r = HalfAdder(1, 0);
        EXPECT_EQ(r.l, 1);
        EXPECT_EQ(r.h, 0);

        r = HalfAdder(1, 1);
        EXPECT_EQ(r.l, 0);
        EXPECT_EQ(r.h, 1);
    }

    TEST(AdderTest, FullAdderTest)
    {
        OneBitAdderResult r = FullAdder(0, 0, 0);
        EXPECT_EQ(r.l, 0);
        EXPECT_EQ(r.h, 0);

        r = FullAdder(0, 0, 1);
        EXPECT_EQ(r.l, 1);
        EXPECT_EQ(r.h, 0);
    
        r = FullAdder(0, 1, 0);
        EXPECT_EQ(r.l, 1);
        EXPECT_EQ(r.h, 0);

        r = FullAdder(0, 1, 1);
        EXPECT_EQ(r.l, 0);
        EXPECT_EQ(r.h, 1);

        r = FullAdder(1, 0, 0);
        EXPECT_EQ(r.l, 1);
        EXPECT_EQ(r.h, 0);

        r = FullAdder(1, 0, 1);
        EXPECT_EQ(r.l, 0);
        EXPECT_EQ(r.h, 1);

        r = FullAdder(1, 1, 0);
        EXPECT_EQ(r.l, 0);
        EXPECT_EQ(r.h, 1);

        r = FullAdder(1, 1, 1);
        EXPECT_EQ(r.l, 1);
        EXPECT_EQ(r.h, 1);
    }

    TEST(AdderTest, Bit16AdderTest)
    {
        uint16_t a = 0x67ac; // 0110 0111 1010 1100
        uint16_t b = 0x7b7b; // 0111 1011 0111 1011
        uint16_t x = 0xe327; // 1110 0011 0010 0111
        bool carry;
        uint16_t y = Bit16Adder(a, b, 0, &carry);
        EXPECT_EQ(x, y);
        EXPECT_EQ(carry, false);

        b = 0xfb7b;
        y = Bit16Adder(a, b, 0, &carry);
        EXPECT_EQ(0x6327, y);
        EXPECT_EQ(carry, true);
    }

    TEST(IncrementTest, Bit16IncrementTest)
    {
        uint16_t n1 = 0x7b7b; // 0111 1011 0111 1011
        bool carryOut;
        uint16_t incrementedN1 = Bit16Increment(n1, &carryOut);
        EXPECT_EQ(incrementedN1, 0x7b7c);
        EXPECT_EQ(carryOut, 0);

        uint16_t n2 = 0xeffff; // 1111 1111 1111 1111
        uint16_t incrementedN2 = Bit16Increment(n2, &carryOut);
        EXPECT_EQ(incrementedN2, 0x0000);
        EXPECT_EQ(carryOut, 1);
    }

    TEST(InvertTtest, Bit16InvertTest)
    {
        uint16_t input = 0x67ac;    // 0110 0111 1010 1100
        uint16_t expected = 0x9853; // 1001 1000 0101 0011
        EXPECT_EQ(expected, Bit16Invert(input));
    }

    TEST(SubtractTest, Bit16SubtractTest)
    {
        uint16_t n1 = 12345;
        uint16_t n2 = 7368;
        EXPECT_EQ(n1 - n2, Bit16Subtract(n1, n2));
        EXPECT_EQ(static_cast<uint16_t>(n2 - n1), Bit16Subtract(n2, n1));
    }

    TEST(ComparisionTest, Bit16EqualToZeroTest)
    {
        EXPECT_EQ(Bit16EqualToZero(0), true);
        EXPECT_EQ(Bit16EqualToZero(123), false);
        EXPECT_EQ(Bit16EqualToZero(-123), false);
    }

    TEST(ComparisionTest, Bit16LessThanZero)
    {
        EXPECT_EQ(Bit16LessThanZero(0), false);
        EXPECT_EQ(Bit16LessThanZero(123), false);
        EXPECT_EQ(Bit16LessThanZero(-123), true);
    }
}