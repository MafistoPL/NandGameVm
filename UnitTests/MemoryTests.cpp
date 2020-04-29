#include "gtest/gtest.h"
#include "memory.h"
#include "gates.h"

// Some cases are "controversial". Need to check which version is correct.
// Nand-Game version, or Ben Eater's version.

// #define NANDGAME

namespace MemoryTests
{
	class TestLatch
	{
	private:
		static const size_t Number_Of_Gates = 5;
		bool prevOutputOfGates[Number_Of_Gates] = {
			0, // A 0
			1, // B 1
			1, // C 2
			1, // D 3
			1  // E 4
		};
	public:
		bool setNewStateAndGetResult(bool st, bool d)
		{
			bool currOutputOfGates[Number_Of_Gates];
			bool outputChanged = false;

			do 
			{
				outputChanged = false;
				currOutputOfGates[0] = Nand(prevOutputOfGates[2], prevOutputOfGates[1]);
				currOutputOfGates[1] = Nand(prevOutputOfGates[0], prevOutputOfGates[3]);
				currOutputOfGates[2] = Nand(st, d);
				currOutputOfGates[3] = Nand(prevOutputOfGates[4], st);
				currOutputOfGates[4] = Not(d);

				for (size_t gateNr = 0; gateNr < Number_Of_Gates; gateNr++)
				{
					if (prevOutputOfGates[gateNr] != currOutputOfGates[gateNr])
					{
						prevOutputOfGates[gateNr] = currOutputOfGates[gateNr];
						outputChanged = true;
					}
				}
			} while (outputChanged == true);
			return currOutputOfGates[0];
		}
	};

#ifdef NANDGAME	
	class TestDataFlipFlop
	{
	private:
		static const size_t Number_Of_Gates = 8;
		bool prevOutputOfGates[Number_Of_Gates] = {
			// Initial state of circuit
			1, // 0
			0, // 1
			1, // 2
			1, // 3
			0, // 4
			0, // 5
			1, // 6
			1  // 7
		};
	public:
		bool setNewStateAndGetResult(bool st, bool d, bool cl)
		{
			bool currOutputOfGates[Number_Of_Gates];
			auto x = sizeof(prevOutputOfGates);
			memcpy(currOutputOfGates, prevOutputOfGates, sizeof(prevOutputOfGates)); // debug code;
			bool outputChanged = false;

			size_t loopCounter = 0;
			do
			{
				if (loopCounter++ > 1000)
				{
					throw "Undefined state";
				}
				outputChanged = false;
				currOutputOfGates[7] = Nand(st, cl);
				currOutputOfGates[6] = Nand(prevOutputOfGates[7], d);
				currOutputOfGates[5] = Not(prevOutputOfGates[7]);
				currOutputOfGates[4] = Nand(prevOutputOfGates[3], prevOutputOfGates[6]);
				currOutputOfGates[3] = Nand(prevOutputOfGates[4], currOutputOfGates[5]);
				currOutputOfGates[2] = Nand(currOutputOfGates[5], prevOutputOfGates[4]);
				currOutputOfGates[1] = Nand(prevOutputOfGates[0], prevOutputOfGates[2]);
				currOutputOfGates[0] = Nand(prevOutputOfGates[7], prevOutputOfGates[1]);

				for (size_t gateNr = 0; gateNr < Number_Of_Gates; gateNr++)
				{
					if (prevOutputOfGates[gateNr] != currOutputOfGates[gateNr])
					{
						prevOutputOfGates[gateNr] = currOutputOfGates[gateNr];
						outputChanged = true;
					}
				}
			} while (outputChanged == true);
			return currOutputOfGates[1];
		}
	};
#else
	class TestDataFlipFlop
	{
	private:
		static const size_t Number_Of_Gates = 10;
		bool prevOutputOfGates[Number_Of_Gates] = {
			// Initial state of circuit
			0, // 0
			1, // 1
			1, // 2
			1, // 3
			1, // 4
			0, // 5
			0, // 6
			1, // 7
			0, // 8
			1  // 9
		};
	public:
		bool setNewStateAndGetResult(bool st, bool d, bool cl)
		{
			bool currOutputOfGates[Number_Of_Gates] = { 0 };
			bool outputChanged = false;

			size_t loopCounter = 0;
			do
			{
				if (loopCounter++ > 1000)
				{
					throw "Undefined state";
				}
				outputChanged = false;
				currOutputOfGates[0] = Nand(prevOutputOfGates[2], prevOutputOfGates[1]);
				currOutputOfGates[1] = Nand(prevOutputOfGates[0], prevOutputOfGates[3]);
				currOutputOfGates[2] = Nand(prevOutputOfGates[5], d);
				currOutputOfGates[3] = Nand(prevOutputOfGates[4], prevOutputOfGates[5]);
				currOutputOfGates[4] = Not(d);
				currOutputOfGates[5] = And(st, prevOutputOfGates[6]);
				currOutputOfGates[6] = And(cl, prevOutputOfGates[7]);
				currOutputOfGates[7] = Not(prevOutputOfGates[8]);
				currOutputOfGates[8] = Not(prevOutputOfGates[9]);
				currOutputOfGates[9] = Not(cl);
				
				for (size_t gateNr = 0; gateNr < Number_Of_Gates; gateNr++)
				{
					if (prevOutputOfGates[gateNr] != currOutputOfGates[gateNr])
					{
						prevOutputOfGates[gateNr] = currOutputOfGates[gateNr];
						outputChanged = true;
					}
				}
			} while (outputChanged == true);
			return currOutputOfGates[0];
		}
	};
#endif

	class TestRegister
	{
	private:
		TestDataFlipFlop dff1, dff2;
	public:
		uint16_t setNewStateAndGetResult(bool st, bool d0, bool d1, bool cl)
		{
			uint16_t result = 0;
			result = dff2.setNewStateAndGetResult(st, d1, cl);
			result <<= 1;
			result |= dff1.setNewStateAndGetResult(st, d0, cl);
			return result;
		}
	};

	// ========================================================================

	TEST(CounterTest, CounterTest)
	{
		Counter counter;
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 1), 1);
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 0), 1);
		
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 1), 2);
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 0), 2);
		
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 1), 3);
		EXPECT_EQ(counter.setNewStateAndGetResult(1, 0, 1), 3);
		
		EXPECT_EQ(counter.setNewStateAndGetResult(1, 0, 0), 3);
		EXPECT_EQ(counter.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 0), 0);

		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 1), 1);
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 0), 1);
		
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 1), 2);
		EXPECT_EQ(counter.setNewStateAndGetResult(0, 0, 0), 2);

		EXPECT_EQ(counter.setNewStateAndGetResult(1, 8, 0), 2);
		EXPECT_EQ(counter.setNewStateAndGetResult(1, 8, 1), 8);
	}

	// ========================================================================

	TEST(RegisterTest, TwoBitRegisterTest)
	{
		TestRegister reg;
		reg.setNewStateAndGetResult(1, 1, 1, 1);
		reg.setNewStateAndGetResult(0, 0, 0, 1);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, 0, 0, 1), 3);
	}

	TEST(RegisterTest, Bit16RegisterTest)
	{
		Register reg;
		uint16_t data = 12345;
		EXPECT_EQ(reg.setNewStateAndGetResult(0, data, 0), 0);
		EXPECT_EQ(reg.setNewStateAndGetResult(0, data, 1), 0);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, data, 1), 0);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, data, 0), 0);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, data, 1), data);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, data, 0), data);
		EXPECT_EQ(reg.setNewStateAndGetResult(0, data, 0), data);
		EXPECT_EQ(reg.setNewStateAndGetResult(0, 0, 0), data);
		EXPECT_EQ(reg.setNewStateAndGetResult(0, 0, 1), data);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, 0, 1), data);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, 0, 0), data);
		EXPECT_EQ(reg.setNewStateAndGetResult(1, 0, 1), 0);
	}

	/*
		Test naming convention is folowing:

		DataFlipFlopTest_abcd_efg

		a - st value before transition
		b - d value before transition
		c - cl value before trasition
		d - output value before transition
		e - st value after transition
		f - d value after transition
		g - cl value after transition
	*/

	TEST(DataFlipFlopTest, DataFlipFlopTest_0000_001)
	{
		TestDataFlipFlop tdff;
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0000_010)
	{
		TestDataFlipFlop tdff;
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0000_011)
	{
		TestDataFlipFlop tdff;
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0000_100)
	{
		TestDataFlipFlop tdff;
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0000_101)
	{
		TestDataFlipFlop tdff;
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0000_110)
	{
		TestDataFlipFlop tdff;
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0000_111)
	{
		TestDataFlipFlop tdff;
#ifdef NANDGAME
		// EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 1, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
#endif
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_0001_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0001_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0001_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0001_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0001_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0001_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0001_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 1, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
#endif
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_0010_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0010_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0010_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0010_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0010_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0010_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0010_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 1, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
#endif
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_0011_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0011_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0011_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0011_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0011_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
#endif
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0011_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0011_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 1, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
#endif
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_0100_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0100_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0100_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0100_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0100_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 0, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
#endif
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0100_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0100_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_0101_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0101_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0101_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0101_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0101_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 0, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
#endif
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0101_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0101_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_0110_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0110_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0110_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0110_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0110_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 0, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
#endif
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0110_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0110_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
#endif
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_0111_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0111_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0111_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0111_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
	}
	
	TEST(DataFlipFlopTest, DataFlipFlopTest_0111_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 0, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
#endif
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0111_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_0111_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1000_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1000_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1000_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1000_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1000_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1000_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1000_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 1, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
#endif
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1001_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1001_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1001_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1001_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1001_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1001_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1001_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 1, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
#endif
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1010_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1010_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1010_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1010_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1010_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1010_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1010_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1011_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1011_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1011_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1011_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1011_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1011_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1011_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1100_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1100_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1100_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1100_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1100_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1100_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 0, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
#endif
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1100_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1101_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
	}
	
	TEST(DataFlipFlopTest, DataFlipFlopTest_1101_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1101_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1101_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1101_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1101_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		// test the interesting transition
#ifdef NANDGAME
		EXPECT_ANY_THROW(tdff.setNewStateAndGetResult(1, 0, 1));
#else
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
#endif
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1101_111)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1110_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1110_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1110_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1110_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 0);
	}
	
	TEST(DataFlipFlopTest, DataFlipFlopTest_1110_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1110_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1110_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 0);
		// test the interesting transition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
	}

	// --------------------------

	TEST(DataFlipFlopTest, DataFlipFlopTest_1111_000)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		// test the interesting trasition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1111_001)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		// test the interesting trasition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1111_010)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		// test the interesting trasition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1111_011)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		// test the interesting trasition
		EXPECT_EQ(tdff.setNewStateAndGetResult(0, 1, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1111_100)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		// test the interesting trasition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 0), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1111_101)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		// test the interesting trasition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 0, 1), 1);
	}

	TEST(DataFlipFlopTest, DataFlipFlopTest_1111_110)
	{
		TestDataFlipFlop tdff;
		// prepare data flip flop state
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 0);
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 1), 1);
		// test the interesting trasition
		EXPECT_EQ(tdff.setNewStateAndGetResult(1, 1, 0), 1);
	}

	// ========================================================================

	TEST(LatchTest, BasicLatchTests)
	{
		Latch latch;
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 0);
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 1);
		/* Result of last test could be unexpected, but it is correct. 
		*/

		TestLatch testLatch;
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 0);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 1);
	}

	// 0, 0 = 0 -> 0, 1 = 0
	TEST(LatchTest, LatchTest000_01)
	{
		Latch latch;
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 0);

		TestLatch testLatch;
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 0);
	}

	// 0, 0 = 0 -> 1, 0 = 0
	TEST(LatchTest, LatchTest000_10)
	{
		Latch latch;
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);

		TestLatch testLatch;
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
	}

	// 0, 0 = 0 -> 1, 1 = 1
	TEST(LatchTest, LatchTest000_11)
	{
		Latch latch;
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);

		TestLatch testLatch;
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
	}

	// 0, 0 = 1 -> 0, 1 = 1
	TEST(LatchTest, LatchTest001_01)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
	}

	// 0, 0 = 1 -> 1, 0 = 0
	TEST(LatchTest, LatchTest001_10)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
	}

	// 0, 0 = 1 -> 1, 1 = 1
	TEST(LatchTest, LatchTest001_11)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
	}

	// 0, 1 = 0 -> 0, 0 = 0
	TEST(LatchTest, LatchTest010_00)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 0);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 0);
	}

	// 0, 1 = 0 -> 1, 1 = 0
	TEST(LatchTest, LatchTest010_11)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
	}

	// 0, 1 = 0 -> 1, 0 = 0
	TEST(LatchTest, LatchTest010_10)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 0);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
	}

	// 0, 1 = 1 -> 0, 0 = 1
	TEST(LatchTest, LatchTest011_00)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 1);
	}

	// 0, 1 = 1 -> 1, 1 = 1
	TEST(LatchTest, LatchTest011_11)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
	}

	// 0, 1 = 1 -> 1, 0 = 0
	TEST(LatchTest, LatchTest011_10)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
	}

	// 1, 0 = 0 -> 1, 1 = 1
	TEST(LatchTest, LatchTest100_11)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
	}

	// 1, 0 = 0 -> 0, 0 = 0
	TEST(LatchTest, LatchTest100_00)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 0);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 0);
	}

	// 1, 0 = 0 -> 0, 1 = 0
	TEST(LatchTest, LatchTest100_01)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 0);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 0);
	}

	// 1, 1 = 1 -> 1, 0 = 0
	TEST(LatchTest, LatchTest111_10)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 0), 0);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 0), 0);
	}

	// 1, 1 = 1 -> 0, 1 = 1
	TEST(LatchTest, LatchTest111_01)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 1), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 1), 1);
	}

	// 1, 1 = 1 -> 0, 0 = 1
	TEST(LatchTest, LatchTest111_00)
	{
		Latch latch;
		// prepare latch
		EXPECT_EQ(latch.setNewStateAndGetResult(1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(latch.setNewStateAndGetResult(0, 0), 1);

		TestLatch testLatch;
		// prepare latch
		EXPECT_EQ(testLatch.setNewStateAndGetResult(1, 1), 1);
		// test the interesting transition
		EXPECT_EQ(testLatch.setNewStateAndGetResult(0, 0), 1);
	}
}
