#include <NazaraUtils/Bitset.hpp>
#include <random>
#include <string>
#include <nanobench.h>

template<typename T>
void TestBitset()
{
	constexpr std::size_t BitsetSize = 1024ull * 1024ull * 8ull;

	ankerl::nanobench::Bench bench;
	bench.minEpochIterations(100);
	bench.title("Using T = Nz::UInt" + std::to_string(sizeof(T) * CHAR_BIT));

	std::minstd_rand gen(42);
	std::uniform_int_distribution<std::size_t> dis(0, BitsetSize - 1);

	bench.run("building a big empty bitset", [&] {
		Nz::Bitset<T> bitset(BitsetSize, false);
		ankerl::nanobench::doNotOptimizeAway(bitset);
	});

	bench.run("building a big full bitset", [&] {
		Nz::Bitset<T> bitset(BitsetSize, true);
		ankerl::nanobench::doNotOptimizeAway(bitset);
	});

	{
		Nz::Bitset<T> bitset(BitsetSize, false);
		bench.run("a single Set", [&] {
			bitset.Set(dis(gen), true);
			ankerl::nanobench::doNotOptimizeAway(bitset);
		});
		bench.run("a single Test", [&] {
			bool r = bitset.Test(dis(gen));
			ankerl::nanobench::doNotOptimizeAway(r);
		});
	}

	{
		Nz::Bitset<T> bitset(BitsetSize, false);
		bench.run("a single UnboundedSet", [&] {
			bitset.UnboundedSet(dis(gen), true);
			ankerl::nanobench::doNotOptimizeAway(bitset);
		});
		bench.run("a single UnboundedTest", [&] {
			bool r = bitset.UnboundedTest(dis(gen));
			ankerl::nanobench::doNotOptimizeAway(r);
		});
	}

	bench.run("building a big bitset with Unbounded methods", [&] {
		Nz::Bitset<T> bitset(BitsetSize, false);
		for (std::size_t i = 0; i < 10000; ++i)
			bitset.UnboundedSet(dis(gen), true);

		ankerl::nanobench::doNotOptimizeAway(bitset);
	});

	bench.run("building a big bitset with Set methods", [&] {
		Nz::Bitset<T> bitset(BitsetSize, false);
		for (std::size_t i = 0; i < 10000; ++i)
			bitset.Set(dis(gen), true);

		ankerl::nanobench::doNotOptimizeAway(bitset);
	});

	// Find first enabled bit
	{
		Nz::Bitset<T> bitset(sizeof(T) * CHAR_BIT, false);
		bitset.Set(sizeof(T) * CHAR_BIT / 2);

		bench.run("find only enabled bit in a single block bitset", [&] {
			std::size_t i = bitset.FindFirst();
			ankerl::nanobench::doNotOptimizeAway(i);
		});

		bench.run("counting bits in a single block bitset", [&] {
			std::size_t count = bitset.Count();
			ankerl::nanobench::doNotOptimizeAway(count);
		});
	}

	{
		Nz::Bitset<T> bitset(BitsetSize, false);
		bitset.Set(BitsetSize / 2 + 21);

		bench.run("find only enabled bit in a big bitset", [&] {
			std::size_t i = bitset.FindFirst();
			ankerl::nanobench::doNotOptimizeAway(i);
		});

		bench.run("counting bits in a big bitset with only one enabled bit", [&] {
			std::size_t count = bitset.Count();
			ankerl::nanobench::doNotOptimizeAway(count);
		});
	}

	{
		Nz::Bitset<T> bitset(BitsetSize, false);
		for (std::size_t i = 0; i < 10000; ++i)
			bitset.Set(dis(gen), true);

		bench.run("iterating on activated bits", [&] {
			for (std::size_t i = bitset.FindFirst(); i != bitset.npos; i = bitset.FindNext(i))
				ankerl::nanobench::doNotOptimizeAway(i);
		});

		bench.run("counting bits in a big bitset with multiple enabled bits", [&] {
			std::size_t count = bitset.Count();
			ankerl::nanobench::doNotOptimizeAway(count);
		});
	}
}

int main()
{
	/*unsigned long foundBit;
	unsigned long mask = 0b0010'0000;

	std::cout << +_BitScanForward(&foundBit, mask) << std::endl;
	std::cout << foundBit << std::endl;

	Nz::Bitset<> bitset("0101011101111000010101010101010111100010101010101010101010101010101011111101");
	Nz::Bitset<> bitset2;
	for (std::size_t i = bitset.FindFirst(); i != bitset.npos; i = bitset.FindNext(i))
	{
		bitset2.UnboundedSet(i);
		std::cout << i << std::endl;
	}

	if (bitset != bitset2)
		throw std::runtime_error("yamashi");

	return 0;*/

	std::srand(42);

	TestBitset<Nz::UInt8>();
	TestBitset<Nz::UInt16>();
	TestBitset<Nz::UInt32>();
	TestBitset<Nz::UInt64>();
}