// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/Endianness.hpp>

namespace Nz
{
	template<typename T>
	constexpr T BigEndianToHost(T value)
	{
#if defined(NAZARA_BIG_ENDIAN)
		return value;
#elif defined(NAZARA_LITTLE_ENDIAN)
		return ByteSwap(value);
#endif
	}

	template<typename T>
	constexpr T LittleEndianToHost(T value)
	{
#if defined(NAZARA_BIG_ENDIAN)
		return ByteSwap(value);
#elif defined(NAZARA_LITTLE_ENDIAN)
		return value;
#endif
	}

	template<typename T>
	constexpr T HostToBigEndian(T value)
	{
		return BigEndianToHost(value);
	}

	template<typename T>
	constexpr T HostToLittleEndian(T value)
	{
		return LittleEndianToHost(value);
	}
}

