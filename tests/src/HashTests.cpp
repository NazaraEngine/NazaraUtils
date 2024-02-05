#include <NazaraUtils/Hash.hpp>
#include <NazaraUtils/TypeName.hpp>
#include <catch2/catch_test_macros.hpp>
#include <cstring>

template<typename T, std::size_t N, typename H>
void TestHash(const char (&str)[N], H expectedHash)
{
	T hash;

	std::size_t size = std::strlen(str);

	INFO("Using " << Nz::TypeName<T>() << " with \"" << str << "\"");
	CHECK(hash(str) == expectedHash);
	CHECK(hash(static_cast<const char*>(str)) == expectedHash);
	CHECK(hash(std::string_view(str, size)) == expectedHash);
	CHECK(hash(reinterpret_cast<const Nz::UInt8*>(str), size) == expectedHash);
}

TEST_CASE("Hash", "[Hash]")
{
	static_assert(Nz::CRC32("Nazara Engine") == 0x8a2f5235u);
	static_assert(Nz::CRC32("t.tv/SirLynixVanFriejtes") == 0xcc2a0914u);
	static_assert(Nz::CRC32("The quick brown fox jumps over the lazy dog") == 0x414fa339u);

	TestHash<Nz::CRC32Hash>("Nazara Engine", 0x8a2f5235u);
	TestHash<Nz::CRC32Hash>("t.tv/SirLynixVanFriejtes", 0xcc2a0914u);
	TestHash<Nz::CRC32Hash>("The quick brown fox jumps over the lazy dog", 0x414fa339u);

	static_assert(Nz::FNV1a32("Nazara Engine") == 0x5ba735a6u);
	static_assert(Nz::FNV1a32("t.tv/SirLynixVanFriejtes") == 0x3ef9d843u);
	static_assert(Nz::FNV1a32("The quick brown fox jumps over the lazy dog") == 0x048fff90u);

	TestHash<Nz::FNV1a32Hash>("Nazara Engine", 0x5ba735a6u);
	TestHash<Nz::FNV1a32Hash>("t.tv/SirLynixVanFriejtes", 0x3ef9d843u);
	TestHash<Nz::FNV1a32Hash>("The quick brown fox jumps over the lazy dog", 0x048fff90u);

	static_assert(Nz::FNV1a64("Nazara Engine") == 0xa00fb3557d90f6e6u);
	static_assert(Nz::FNV1a64("t.tv/SirLynixVanFriejtes") == 0x4d2631a6429ff643u);
	static_assert(Nz::FNV1a64("The quick brown fox jumps over the lazy dog") == 0xf3f9b7f5e7e47110u);

	TestHash<Nz::FNV1a64Hash>("Nazara Engine", 0xa00fb3557d90f6e6u);
	TestHash<Nz::FNV1a64Hash>("t.tv/SirLynixVanFriejtes", 0x4d2631a6429ff643u);
	TestHash<Nz::FNV1a64Hash>("The quick brown fox jumps over the lazy dog", 0xf3f9b7f5e7e47110u);
}
