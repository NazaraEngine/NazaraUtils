#include <NazaraUtils/Bitset.hpp>
#include <random>
#include <string>
#include <nanobench.h>

template<typename T>
void TestBitOp()
{
	ankerl::nanobench::Bench bench;
	bench.minEpochIterations(1000);
	bench.title("Using T = Nz::UInt" + std::to_string(sizeof(T) * CHAR_BIT));

	std::minstd_rand gen(std::random_device{}());
	std::uniform_int_distribution<T> dis(0, std::numeric_limits<T>::max());

	T randomValue = dis(gen);
	ankerl::nanobench::doNotOptimizeAway(randomValue);

	bench.run("finding the first active bit", [&] {
		unsigned int fsb = Nz::FindFirstBit(randomValue);
		ankerl::nanobench::doNotOptimizeAway(fsb);
	});
	
	bench.run("integral log2", [&] {
		unsigned int fsb = Nz::IntegralLog2(randomValue);
		ankerl::nanobench::doNotOptimizeAway(fsb);
	});

	bench.run("integral log2 (power of two)", [&] {
		unsigned int fsb = Nz::IntegralLog2Pot(randomValue & ~randomValue);
		ankerl::nanobench::doNotOptimizeAway(fsb);
	});
}

int main()
{
	TestBitOp<Nz::UInt16>();
	TestBitOp<Nz::UInt32>();
	TestBitOp<Nz::UInt64>();
}