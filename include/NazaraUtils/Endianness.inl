// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/Algorithm.hpp>

namespace Nz
{
	/*!
	* \ingroup utils
	* \brief Gets the platform endianness
	* \return Type of the endianness
	*/
	constexpr Endianness GetPlatformEndianness()
	{
		#if defined(NAZARA_BIG_ENDIAN)
		return Endianness::BigEndian;
		#elif defined(NAZARA_LITTLE_ENDIAN)
		return Endianness::LittleEndian;
		#endif
	}
}

