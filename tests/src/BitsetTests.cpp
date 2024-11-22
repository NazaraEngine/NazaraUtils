#include <NazaraUtils/Algorithm.hpp>
#include <NazaraUtils/Bitset.hpp>
#include <catch2/catch_test_macros.hpp>
#include <array>
#include <string>
#include <iostream>

template<typename Bitset> void Check(const char* title);
template<typename Bitset> void CheckAppend(const char* title);
template<typename Bitset> void CheckBitOps(const char* title);
template<typename Bitset> void CheckBitOpsMultipleBlocks(const char* title);
template<typename Bitset> void CheckConstructor(const char* title);
template<typename Bitset> void CheckCopyMoveSwap(const char* title);
template<typename Bitset> void CheckIter(const char* title);
template<typename Bitset> void CheckRead(const char* title);
template<typename Bitset> void CheckResize(const char* title);
template<typename Bitset> void CheckReverse(const char* title);

template<typename Bitset>
struct IsUsingVector : std::false_type {};

template<typename Block>
struct IsUsingVector<Nz::Bitset<Block, std::vector<Block>>> : std::true_type {};

template<typename Bitset>
struct IsUsingDynamicCapacity : std::true_type {};

template<typename Block, std::size_t Capacity>
struct IsUsingDynamicCapacity<Nz::Bitset<Block, Nz::FixedVector<Block, Capacity, void>>> : std::false_type {};

SCENARIO("Bitset", "[CORE][BITSET]")
{
	Check<Nz::Bitset<Nz::UInt8>>("Bitset made of 8bits blocks");
	Check<Nz::Bitset<Nz::UInt16>>("Bitset made of 16bits blocks");
	Check<Nz::Bitset<Nz::UInt32>>("Bitset made of 32bits blocks");
	Check<Nz::Bitset<Nz::UInt64>>("Bitset made of 64bits blocks");

	Check<Nz::FixedBitset<Nz::UInt8, 256>>("FixedBitset made of 8bits blocks with a capacity of 256");
	Check<Nz::FixedBitset<Nz::UInt16, 256>>("FixedBitset made of 16bits blocks with a capacity of 256");
	Check<Nz::FixedBitset<Nz::UInt32, 256>>("FixedBitset made of 32bits blocks with a capacity of 256");
	Check<Nz::FixedBitset<Nz::UInt64, 256>>("FixedBitset made of 64bits blocks with a capacity of 256");

	Check<Nz::HybridBitset<Nz::UInt8, 32>>("HybridBitset made of 8bits blocks with a capacity of 32");
	Check<Nz::HybridBitset<Nz::UInt16, 32>>("HybridBitset made of 16bits blocks with a capacity of 32");
	Check<Nz::HybridBitset<Nz::UInt32, 32>>("HybridBitset made of 32bits blocks with a capacity of 32");
	Check<Nz::HybridBitset<Nz::UInt64, 32>>("HybridBitset made of 64bits blocks with a capacity of 32");

	Check<Nz::HybridBitset<Nz::UInt8, 256>>("HybridBitset made of 8bits blocks with a capacity of 256");
	Check<Nz::HybridBitset<Nz::UInt16, 256>>("HybridBitset made of 16bits blocks with a capacity of 256");
	Check<Nz::HybridBitset<Nz::UInt32, 256>>("HybridBitset made of 32bits blocks with a capacity of 256");
	Check<Nz::HybridBitset<Nz::UInt64, 256>>("HybridBitset made of 64bits blocks with a capacity of 256");
}

template<typename Bitset>
void Check(const char* title)
{
	CheckResize<Bitset>(title);

	CheckConstructor<Bitset>(title);
	CheckCopyMoveSwap<Bitset>(title);

	CheckBitOps<Bitset>(title);
	CheckBitOpsMultipleBlocks<Bitset>(title);

	CheckAppend<Bitset>(title);
	CheckRead<Bitset>(title);
	CheckReverse<Bitset>(title);

	CheckIter<Bitset>(title);
}

template<typename Bitset>
void CheckAppend(const char* title)
{
	SECTION(title)
	{
		GIVEN("An empty bitset filled by bytes")
		{
			#define BitVal1 00110111
			#define BitVal2 11011110
			#define BitVal3 01000010
			std::array<Nz::UInt8, 3> data = {{NazaraPrefixMacro(BitVal1, 0b), NazaraPrefixMacro(BitVal2, 0b), NazaraPrefixMacro(BitVal3, 0b)}};
			const char result[] = NazaraStringifyMacro(BitVal3) NazaraStringifyMacro(BitVal2) NazaraStringifyMacro(BitVal1);
			std::size_t bitCount = data.size() * 8;
			#undef BitVal1
			#undef BitVal2
			#undef BitVal3

			std::array<std::pair<const char*, std::size_t>, 7> tests = {
				{
					{"We append bits one by one", 1},
					{"We append bits two by two", 2},
					{"We append bits three by three", 3},
					{"We append bits four by four", 4},
					{"We append bits six by six", 6},
					{"We append bits byte by byte", 8},
					{"We append bits twelve by twelve", 12}
				}
			};

			for (auto& pair : tests)
			{
				WHEN(pair.first)
				{
					Bitset bitset;

					for (std::size_t i = 0; i < bitCount; i += pair.second)
					{
						Nz::UInt16 value = data[i / 8] >> (i % 8);
						if ((i % 8) + pair.second > 8 && i/8 != data.size()-1)
							value |= static_cast<Nz::UInt16>(data[i / 8 + 1]) << (8 - (i % 8));

						bitset.AppendBits(value, pair.second);
					}

					REQUIRE(bitset.GetSize() == bitCount);

					Bitset expectedBitset(result);

					CHECK(bitset == expectedBitset);
					CHECK(bitset.GetBlockCount() == (bitCount / bitset.bitsPerBlock + std::min<std::size_t>(1, bitCount % bitset.bitsPerBlock)));
				}
			}
		}
	}
}

template<typename Bitset>
void CheckBitOps(const char* title)
{
	SECTION(title)
	{
		GIVEN("Two bitsets")
		{
			Bitset first("01001");
			Bitset second("10111");

			WHEN("We perform operators")
			{
				Bitset andBitset = first & second;
				Bitset orBitset = first | second;
				Bitset xorBitset = first ^ second;

				THEN("They should operate as logical operators")
				{
					CHECK(andBitset == Bitset("00001"));
					CHECK(orBitset  == Bitset("11111"));
					CHECK(orBitset.TestAll());
					CHECK(xorBitset == Bitset("11110"));
					CHECK(!xorBitset.TestAll());
					CHECK((~orBitset).TestNone());
				}
			}

			WHEN("We perform bit shifts")
			{
				first.ShiftLeft(1);
				second.ShiftRight(2);

				THEN("We should obtain these")
				{
					CHECK(first == Bitset("10010"));
					CHECK(second == Bitset("101"));
				}
			}
		}
	}
}

template<typename Bitset>
void CheckBitOpsMultipleBlocks(const char* title)
{
	SECTION(title)
	{
		GIVEN("Two bitsets")
		{
			Bitset first("01001011010010101001010011010101001");
			Bitset second("10111111101101110110111101101110110");

			WHEN("We perform operators")
			{
				Bitset andBitset = first & second;
				Bitset orBitset = first | second;
				Bitset xorBitset = first ^ second;

				THEN("They should operate as logical operators")
				{
					CHECK(andBitset == Bitset("00001011000000100000010001000100000"));
					CHECK(orBitset  == Bitset("11111111111111111111111111111111111"));
					CHECK(orBitset.TestAll());
					CHECK(xorBitset == Bitset("11110100111111011111101110111011111"));
					CHECK(!xorBitset.TestAll());
					CHECK((~orBitset).TestNone());
				}
			}

			WHEN("We perform bit shifts")
			{
				first.ShiftLeft(16);
				second.ShiftRight(16);

				THEN("We should obtain these")
				{
					CHECK(first == Bitset("10010100110101010010000000000000000"));
					first.ShiftLeft(1);
					CHECK(first == Bitset("00101001101010100100000000000000000"));
					CHECK(second == Bitset("1011111110110111011"));
					second.ShiftRight(1);
					CHECK(second == Bitset("101111111011011101"));
				}
			}
		}
	}
}

template<typename Bitset>
void CheckConstructor(const char* title)
{
	SECTION(title)
	{
		GIVEN("Allocate and constructor")
		{
			Bitset bitset(3, false);

			THEN("Capacity is 3 and size is 3")
			{
				CHECK(bitset.GetSize() == 3);
				CHECK(bitset.GetCapacity() >= 3);
			}
		}

		GIVEN("Iterator and default constructor")
		{
			std::string anotherDataString("0101");
			Bitset defaultByte;
			Bitset anotherData(anotherDataString.c_str());

			WHEN("We assign 'anotherData'")
			{
				defaultByte = anotherDataString;
				CHECK(anotherData == defaultByte);
				CHECK(defaultByte.GetSize() == 4);
				CHECK(defaultByte.GetCapacity() >= 4);
				CHECK(anotherData.GetSize() == 4);
				CHECK(anotherData.GetCapacity() >= 4);
			}
		}
	}
}

template<typename Bitset>
void CheckCopyMoveSwap(const char* title)
{
	SECTION(title)
	{
		GIVEN("Copy and Move constructor")
		{
			Bitset originalArray(3, true);

			WHEN("We copy")
			{
				Bitset copyBitset(originalArray);

				THEN("We get a copy")
				{
					CHECK(copyBitset == originalArray);

					AND_WHEN("We modify one")
					{
						for (std::size_t i = 0; i < copyBitset.GetSize(); ++i)
							copyBitset[i] = false;

						THEN("They are no more equal")
						{
							CHECK(copyBitset != originalArray);
							CHECK(copyBitset == Bitset(3, false));
						}
					}
				}
			}

			WHEN("We move")
			{
				Bitset moveBitset(std::move(originalArray));

				THEN("These results are expected")
				{
					CHECK(moveBitset == Bitset(3, true));
					if constexpr (IsUsingVector<Bitset>())
						CHECK(originalArray.GetCapacity() == 0);
				}
			}
		}

		GIVEN("Three bitsets")
		{
			Bitset first("01001");
			Bitset second("10110");
			Bitset third;

			WHEN("We swap first and third, then second and third and finally third and first")
			{
				Bitset oldFirst(first);
				Bitset oldSecond(second);

				first.Swap(third);
				std::swap(second, third);
				third.Swap(first);

				THEN("First and second have been swapped and third is still empty.")
				{
					CHECK(oldFirst == second);
					CHECK(oldSecond == first);
					CHECK(third.GetSize() == 0);
				}
			}
		}
	}
}

template<typename Bitset>
void CheckIter(const char* title)
{
	SECTION(title)
	{
		GIVEN("A bitset")
		{
			Bitset block("0101000001100101011101000110100101110100001000000110011001101111011101010110100101101110011001010111010101110010001000000011101100101001");
			Bitset foundBlock(block.GetSize(), false);
			REQUIRE_FALSE(foundBlock.TestAny());
			for (std::size_t bit : block.IterBits())
			{
				CHECK(block[bit]);
				CHECK(!foundBlock[bit]);
				foundBlock[bit] = true;
			}

			foundBlock ^= block;
			CHECK(foundBlock.TestNone());
		}
	}
}

template<typename Bitset>
void CheckRead(const char* title)
{
	SECTION(title)
	{
		GIVEN("An empty bitset filled by reading")
		{
			#define BitVal1 10010101
			#define BitVal2 11010010
			#define BitVal3 01101010
			std::array<Nz::UInt8, 3> data = {{NazaraPrefixMacro(BitVal1, 0b), NazaraPrefixMacro(BitVal2, 0b), NazaraPrefixMacro(BitVal3, 0b)}};
			const char result[] = NazaraStringifyMacro(BitVal3) NazaraStringifyMacro(BitVal2) NazaraStringifyMacro(BitVal1);
			std::size_t bitCount = data.size() * 8;
			#undef BitVal1
			#undef BitVal2
			#undef BitVal3

			std::array<std::pair<const char*, std::size_t>, 8> tests = {
				{
					{"We read bits one by one", 1},
					{"We read bits two by two", 2},
					{"We read bits three by three", 3},
					{"We read bits four by four", 4},
					{"We read bits six by six", 6},
					{"We read bits byte by byte", 8},
					{"We read bits twelve by twelve", 12},
					{"We read bits all at once", 24}
				}
			};

			for (auto& pair : tests)
			{
				WHEN(pair.first)
				{
					Bitset bitset;

					auto seq = bitset.Write(data.data(), pair.second);
					for (std::size_t i = pair.second; i < bitCount; i += pair.second)
						seq = bitset.Write(seq, pair.second);

					REQUIRE(bitset.GetSize() == bitCount);

					Bitset expectedBitset(result);

					CHECK(bitset == expectedBitset);
					CHECK(bitset.GetBlockCount() == (bitCount / bitset.bitsPerBlock + std::min<std::size_t>(1, bitCount % bitset.bitsPerBlock)));
				}
			}
		}
	}
}

template<typename Bitset>
void CheckResize(const char* title)
{
	SECTION(title)
	{
		GIVEN("An empty bitset")
		{
			Bitset bitset;

			CHECK(bitset.GetSize() == 0);

			std::vector<std::size_t> sizes;
			if constexpr (IsUsingDynamicCapacity<Bitset>())
				sizes = { 0, 1, 5, 63, 64, 142, 1024, 1024 * 1024 };
			else
				sizes = { 0, 1, 5, 63, 64, 89 };

			for (std::size_t size : sizes)
			{
				WHEN("we resize it to " << size)
				{
					bitset.Resize(size);
					CHECK(bitset.GetSize() == size);
					if (size > 0)
					{
						CHECK(bitset[size / 2] == false);

						AND_WHEN("we resize it to " << (size * 2))
						{
							bitset.Resize(size * 2, true);
							CHECK(bitset[size - 1] == false);
							CHECK(bitset[size] == true);
						}
					}
				}

				WHEN("we resize it to " << size << " with a default value")
				{
					bitset.Resize(size, true);
					CHECK(bitset.GetSize() == size);
					if (size > 0)
					{
						CHECK(bitset[size / 2] == true);

						AND_WHEN("we resize it to " << (size * 2))
						{
							bitset.Resize(size * 2, false);
							CHECK(bitset[size - 1] == true);
							CHECK(bitset[size] == false);
						}
					}
				}
			}
		}
	}
}

template<typename Bitset>
void CheckReverse(const char* title)
{
	SECTION(title)
	{
		GIVEN("A bitset")
		{
			std::string bits = "010011100010001101001111";
			Bitset expected(bits);

			WHEN("We reverse the order of bits")
			{
				Bitset bitset(bits);
				bitset.Reverse();

				THEN("The order of bits should be reversed")
				{
					std::string reversedBits = bits;
					std::reverse(reversedBits.begin(), reversedBits.end());
					CHECK(bitset == Bitset(reversedBits));
				}
				AND_WHEN("We reverse the bit order again")
				{
					bitset.Reverse();

					THEN("It should be back to normal")
					{
						CHECK(bitset == expected);
					}
				}
			}
		}
	}
}
