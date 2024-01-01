// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <array>

namespace Nz
{
	namespace Detail
	{
		// https://stackoverflow.com/questions/28675727/using-crc32-algorithm-to-hash-string-at-compile-time
		// Generates CRC-32 table, algorithm based from this link:
		// http://www.hackersdelight.org/hdcodetxt/crc.c.txt
		constexpr auto GenerateCRC32Table(UInt32 polynomial = 0xEDB88320)
		{
#ifdef NAZARA_COMPILER_MSVC
// Disable warning: unary minus operator applied to unsigned type, result still unsigned
#pragma warning(push)
#pragma warning(disable: 4146)
#endif

			constexpr UInt32 byteCount = 256;
			constexpr UInt32 iterationCount = 8;

			std::array<UInt32, byteCount> crc32Table{};
			for (UInt32 byte = 0u; byte < byteCount; ++byte)
			{
				UInt32 crc = byte;

				for (UInt32 i = 0; i < iterationCount; ++i)
				{
					UInt32 mask = static_cast<UInt32>(-(crc & 1));
					crc = (crc >> 1) ^ (polynomial & mask);
				}

				crc32Table[byte] = crc;
			}

			return crc32Table;

#ifdef NAZARA_COMPILER_MSVC
#pragma warning(pop)
#endif
		}

		// Stores CRC-32 table and softly validates it.
		static constexpr auto s_crc32Table = GenerateCRC32Table();
		static_assert(
			s_crc32Table.size() == 256 &&
			s_crc32Table[1] == 0x77073096 &&
			s_crc32Table[255] == 0x2D02EF8D,
			"GenerateCRC32Table generated unexpected result."
		);

		constexpr UInt32 FNV1OffsetBasis_32 = 0x811c9dc5u;
		constexpr UInt32 FNV1Prime_32 = 0x1000193u;

		constexpr UInt64 FNV1OffsetBasis_64 = 0xcbf29ce484222325ull;
		constexpr UInt64 FNV1Prime_64 = 0x100000001b3ull;
	}

	/*!
	* \ingroup utils
	* \brief Combines two hash in one
	*
	* \param seed First value that will be modified (expected to be 64bits)
	* \param v Second value to hash
	*/
	// Algorithm from CityHash by Google
	// http://stackoverflow.com/questions/8513911/how-to-create-a-good-hash-combine-with-64-bit-output-inspired-by-boosthash-co
	template<typename T>
	void HashCombine(std::size_t& seed, const T& v)
	{
		constexpr UInt64 kMul = 0x9ddfea08eb382d69ULL;

		std::hash<T> hasher;
		UInt64 a = (hasher(v) ^ seed) * kMul;
		a ^= (a >> 47);

		UInt64 b = (seed ^ a) * kMul;
		b ^= (b >> 47);

		seed = static_cast<std::size_t>(b * kMul);
	}

	template<typename T, typename... Args>
	std::size_t HashCombine(const T& v, const Args&... args)
	{
		std::size_t hash = std::hash<T>{}(v);

		if constexpr (sizeof...(Args) > 0)
			HashCombine(hash, HashCombine(args...));

		return hash;
	}

	template<typename T>
	template<typename... Args>
	std::size_t HashFunctor<T>::operator()(Args&&... args)
	{
		return static_cast<std::size_t>(T{}(std::forward<Args>(args)...));
	}


	// From https://stackoverflow.com/questions/28675727/using-crc32-algorithm-to-hash-string-at-compile-time
	constexpr UInt32 CRC32(const char* str) noexcept
	{
		UInt32 crc = 0xFFFFFFFFu;

		for (std::size_t i = 0u; str[i]; ++i)
			crc = Detail::s_crc32Table[(crc ^ str[i]) & 0xFF] ^ (crc >> 8);

		return ~crc;
	}

	constexpr UInt32 CRC32(std::string_view str) noexcept
	{
		UInt32 crc = 0xFFFFFFFFu;

		for (std::size_t i = 0u; i < str.size(); ++i)
			crc = Detail::s_crc32Table[(crc ^ str[i]) & 0xFF] ^ (crc >> 8);

		return ~crc;
	}

	constexpr UInt32 CRC32(const UInt8* input, std::size_t size) noexcept
	{
		UInt32 crc = 0xFFFFFFFFu;

		for (std::size_t i = 0u; i < size; ++i)
			crc = Detail::s_crc32Table[(crc ^ input[i]) & 0xFF] ^ (crc >> 8);

		return ~crc;
	}

	template<std::size_t N>
	constexpr UInt32 CRC32(const char(&str)[N]) noexcept
	{
		return CRC32(std::string_view(str, N));
	}

	template<typename... Args>
	UInt32 CRC32Hash::operator()(Args&&... args)
	{
		return CRC32(std::forward<Args>(args)...);
	}


	constexpr UInt32 FNV1a32(const char* str) noexcept
	{
		UInt32 hash = Detail::FNV1OffsetBasis_32;

		for (std::size_t i = 0u; str[i]; ++i)
		{
			hash ^= UInt32(str[i]);
			hash *= Detail::FNV1Prime_32;
		}

		return hash;
	}

	constexpr UInt32 FNV1a32(std::string_view str) noexcept
	{
		UInt32 hash = Detail::FNV1OffsetBasis_32;

		for (std::size_t i = 0u; i < str.size(); ++i)
		{
			hash ^= UInt32(str[i]);
			hash *= Detail::FNV1Prime_32;
		}

		return hash;
	}

	constexpr UInt32 FNV1a32(const UInt8* data, std::size_t size) noexcept
	{
		UInt32 hash = Detail::FNV1OffsetBasis_32;

		for (std::size_t i = 0u; i < size; ++i)
		{
			hash ^= data[i];
			hash *= Detail::FNV1Prime_32;
		}

		return hash;
	}

	template<std::size_t N>
	constexpr UInt32 FNV1a32(const char(&str)[N]) noexcept
	{
		return FNV1a32(std::string_view(str, N));
	}

	template<typename... Args>
	UInt32 FNV1a32Hash::operator()(Args&&... args)
	{
		return FNV1a32(std::forward<Args>(args)...);
	}


	constexpr UInt64 FNV1a64(const char* str) noexcept
	{
		UInt64 hash = Detail::FNV1OffsetBasis_64;

		for (std::size_t i = 0u; str[i]; ++i)
		{
			hash ^= UInt64(str[i]);
			hash *= Detail::FNV1Prime_64;
		}

		return hash;
	}

	constexpr UInt64 FNV1a64(std::string_view str) noexcept
	{
		UInt64 hash = Detail::FNV1OffsetBasis_64;

		for (std::size_t i = 0u; i < str.size(); ++i)
		{
			hash ^= UInt64(str[i]);
			hash *= Detail::FNV1Prime_64;
		}

		return hash;
	}

	constexpr UInt64 FNV1a64(const UInt8* data, std::size_t size) noexcept
	{
		UInt64 hash = Detail::FNV1OffsetBasis_64;

		for (std::size_t i = 0u; i < size; ++i)
		{
			hash ^= data[i];
			hash *= Detail::FNV1Prime_64;
		}

		return hash;
	}

	template<std::size_t N>
	constexpr UInt64 FNV1a64(const char(&str)[N]) noexcept
	{
		return FNV1a64(std::string_view(str, N));
	}

	template<typename... Args>
	UInt64 FNV1a64Hash::operator()(Args&&... args)
	{
		return FNV1a64(std::forward<Args>(args)...);
	}
}

