#include "gtest/gtest.h"
#include "gates.h"

namespace LogicGateTests
{
    TEST(GateTest, NandTest)
    {
        EXPECT_EQ(1, GateNand(0, 0));
        EXPECT_EQ(1, GateNand(0, 1));
        EXPECT_EQ(1, GateNand(1, 0));
        EXPECT_EQ(0, GateNand(1, 1));
    }

    TEST(BlackboxGateTest, NandTest)
    {
        EXPECT_EQ(1, Nand(0, 0));
        EXPECT_EQ(1, Nand(0, 1));
        EXPECT_EQ(1, Nand(1, 0));
        EXPECT_EQ(0, Nand(1, 1));
    }

    TEST(GateTest, NotTest)
    {
        EXPECT_EQ(1, GateNot(0));
        EXPECT_EQ(0, GateNot(1));
    }

    TEST(BlackboxGateTest, NotTest)
    {
        EXPECT_EQ(1, Not(0));
        EXPECT_EQ(0, Not(1));
    }

    TEST(GateTest, AndTest)
    {
        EXPECT_EQ(0, GateAnd(0, 0));
        EXPECT_EQ(0, GateAnd(0, 1));
        EXPECT_EQ(0, GateAnd(1, 0));
        EXPECT_EQ(1, GateAnd(1, 1));
    }

    TEST(BlackboxGateTest, AndTest)
    {
        EXPECT_EQ(0, And(0, 0));
        EXPECT_EQ(0, And(0, 1));
        EXPECT_EQ(0, And(1, 0));
        EXPECT_EQ(1, And(1, 1));
    }

    TEST(GateTest, OrTest)
    {
        EXPECT_EQ(0, GateOr(0, 0));
        EXPECT_EQ(1, GateOr(0, 1));
        EXPECT_EQ(1, GateOr(1, 0));
        EXPECT_EQ(1, GateOr(1, 1));
    }

    TEST(BlackboxGateTest, OrTest)
    {
        EXPECT_EQ(0, Or(0, 0));
        EXPECT_EQ(1, Or(0, 1));
        EXPECT_EQ(1, Or(1, 0));
        EXPECT_EQ(1, Or(1, 1));
    }

    TEST(GateTest, XorTest)
    {
        EXPECT_EQ(0, GateXor(0, 0));
        EXPECT_EQ(1, GateXor(0, 1));
        EXPECT_EQ(1, GateXor(1, 0));
        EXPECT_EQ(0, GateXor(1, 1));
    }

    TEST(BlackboxGateTest, XorTest)
    {
        EXPECT_EQ(0, Xor(0, 0));
        EXPECT_EQ(1, Xor(0, 1));
        EXPECT_EQ(1, Xor(1, 0));
        EXPECT_EQ(0, Xor(1, 1));
    }
}