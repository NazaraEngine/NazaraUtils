// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utils/Endianness.hpp>
#include <algorithm>

namespace Nz
{
	/*!
	* \ingroup utils
	* \brief Gets the platform endianness
	* \return Type of the endianness
	*/
	inline constexpr Endianness GetPlatformEndianness()
	{
		#if defined(NAZARA_BIG_ENDIAN)
		return Endianness::BigEndian;
		#elif defined(NAZARA_LITTLE_ENDIAN)
		return Endianness::LittleEndian;
		#endif
	}

	/*!
	* \ingroup utils
	* \brief Swaps the byte for endianness operations
	*
	* \param buffer Raw memory
	* \param size Size to change endianness
	*
	* \remark If size is greater than the preallocated buffer, the behavior is undefined
	*/
	inline void SwapBytes(void* buffer, std::size_t size)
	{
		UInt8* bytes = static_cast<UInt8*>(buffer);
		std::size_t i = 0;
		std::size_t j = size - 1;

		while (i < j)
			std::swap(bytes[i++], bytes[j--]);
	}

	template<typename T> 
	T SwapBytes(T value)
	{
		SwapBytes(&value, sizeof(T));
		return value;
	}
}

