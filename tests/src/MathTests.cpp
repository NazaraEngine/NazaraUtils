#include <NazaraUtils/MathUtils.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

// Constexprness test
#ifdef NAZARA_HAS_CONSTEVAL
static_assert(Nz::CountBits(65) == 2);
static_assert(Nz::FindFirstBit(0) == 0);
static_assert(Nz::FindFirstBit(0b00110101) == 1);
static_assert(Nz::FindFirstBit(0b00110100) == 3);
#endif

static_assert(Nz::SetBit(0b00110001, 1) == 0b00110011);
static_assert(Nz::TestBit(0b00110001, 0));
static_assert(!Nz::TestBit(0b00110001, 1));

template<typename T>
void TestCountBits()
{
	T value = 0;
	CHECK(Nz::CountBits(value) == 0);
	CHECK(Nz::CountBits(std::numeric_limits<T>::max()) == Nz::BitCount<T>());
	CHECK(Nz::CountBits(std::numeric_limits<T>::max() / 2) == Nz::BitCount<T>() - 1);

	for (std::size_t i = 0; i < Nz::BitCount<T>(); ++i)
	{
		value = T(1) << i;
		CHECK(Nz::CountBits(value) == 1);
	}
}

template<typename T>
void TestFindFirstBit()
{
	T value = 0;
	CHECK(Nz::FindFirstBit(value) == 0);

	for (std::size_t i = 0; i < Nz::BitCount<T>(); ++i)
	{
		value = T(1) << i;
		CHECK(Nz::FindFirstBit(value) == i + 1);
	}
}

SCENARIO("MathUtils", "[MathUtils]")
{
	WHEN("Testing ByteSwap")
	{
		static_assert(Nz::ByteSwap(std::int16_t(0xABCD)) == std::int16_t(0xCDAB));
		static_assert(Nz::ByteSwap(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
		static_assert(Nz::ByteSwap(std::int32_t(0xABCDEF01)) == std::int32_t(0x01EFCDAB));
		static_assert(Nz::ByteSwap(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
		static_assert(Nz::ByteSwap(std::int64_t(0xABCDEF0102030405)) == std::int64_t(0x0504030201EFCDAB));
		static_assert(Nz::ByteSwap(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));

		CHECK(Nz::ByteSwap(std::int16_t(0xABCD)) == std::int16_t(0xCDAB));
		CHECK(Nz::ByteSwap(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
		CHECK(Nz::ByteSwap(std::int32_t(0xABCDEF01)) == std::int32_t(0x01EFCDAB));
		CHECK(Nz::ByteSwap(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
		CHECK(Nz::ByteSwap(std::int64_t(0xABCDEF0102030405)) == std::int64_t(0x0504030201EFCDAB));
		CHECK(Nz::ByteSwap(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));

#ifdef NAZARA_HAS_CONSTEXPR_BITCAST
		static_assert(Nz::BitCast<std::uint32_t>(Nz::ByteSwap(Nz::BitCast<float>(0xABCDEF01))) == std::uint32_t(0x01EFCDAB));
		static_assert(Nz::BitCast<std::uint64_t>(Nz::ByteSwap(Nz::BitCast<double>(0xABCDEF0102030405))) == std::uint64_t(0x0504030201EFCDAB));
#endif

		CHECK(Nz::BitCast<std::uint32_t>(Nz::ByteSwap(Nz::BitCast<float>(0xABCDEF01))) == std::uint32_t(0x01EFCDAB));
		CHECK(Nz::BitCast<std::uint64_t>(Nz::ByteSwap(Nz::BitCast<double>(0xABCDEF0102030405))) == std::uint64_t(0x0504030201EFCDAB));
	}

	WHEN("Testing CountBit")
	{
		TestCountBits<Nz::UInt8>();
		TestCountBits<Nz::UInt16>();
		TestCountBits<Nz::UInt32>();
		TestCountBits<Nz::UInt64>();
	}

	WHEN("Testing FindFirstBit")
	{
		TestFindFirstBit<Nz::UInt8>();
		TestFindFirstBit<Nz::UInt16>();
		TestFindFirstBit<Nz::UInt32>();
		TestFindFirstBit<Nz::UInt64>();
	}

	WHEN("Testing Mod")
	{
#ifdef NAZARA_HAS_CONSTEVAL
		static_assert(Nz::Mod(3, 2) == 1);
		static_assert(Nz::NumberEquals(Nz::Mod(3.f, 2.f), 1.f, 0.00001f));
#endif

		CHECK(Nz::Mod(3, 2) == 1);
		CHECK(Nz::Mod(3.f, 2.f) == Catch::Approx(1.f));
	}

	WHEN("Testing RoundToPow2")
	{
		static_assert(Nz::RoundToPow2(2u) == 2u);
		static_assert(Nz::RoundToPow2(7u) == 8u);
		static_assert(Nz::RoundToPow2(24u) == 32u);
		static_assert(Nz::RoundToPow2(3999) == 4096);
		static_assert(Nz::RoundToPow2(4096) == 4096);
		static_assert(Nz::RoundToPow2(2000000000u) == 2147483648u);
		static_assert(Nz::RoundToPow2(4000000000ull) == 4294967296ull);
		static_assert(Nz::RoundToPow2(9000000000000000000ull) == 9223372036854775808ull);

		CHECK(Nz::RoundToPow2(2u) == 2u);
		CHECK(Nz::RoundToPow2(7u) == 8u);
		CHECK(Nz::RoundToPow2(24u) == 32u);
		CHECK(Nz::RoundToPow2(24u) == 32u);
		CHECK(Nz::RoundToPow2(4000) == 4096);
		CHECK(Nz::RoundToPow2(32769) == 65536);
		CHECK(Nz::RoundToPow2(2000000000u) == 2147483648u);
		CHECK(Nz::RoundToPow2(4000000000ull) == 4294967296ull);
		CHECK(Nz::RoundToPow2(9000000000000000000ull) == 9223372036854775808ull);
	}
}
