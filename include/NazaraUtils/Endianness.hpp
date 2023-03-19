// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_ENDIANNESS_HPP
#define NAZARA_UTILS_ENDIANNESS_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <cstddef>

#if __has_include(<endian.h>)
#include <endian.h>
#elif __has_include(<sys/endian.h>)
#include <sys/endian.h>
#endif

#if !defined(NAZARA_BIG_ENDIAN) && !defined(NAZARA_LITTLE_ENDIAN)
	// Automatic detection following macros of compiler
	#if defined(__m68k__) || defined(mc68000) || defined(_M_M68K) || (defined(__MIPS__) && defined(__MISPEB__)) || \
	    defined(__ppc__) || defined(__POWERPC__) || defined(_M_PPC) || defined(__sparc__) || defined(__hppa__) || \
	    defined(__BIG_ENDIAN__) || defined(__ARMEB__) || defined(__THUMBEB__) || defined(__AARCH64EB__) || defined(_MIPSEB) || \
	    defined(__MIPSEB) || defined(__MIPSEB__) || \
	    (defined(__BYTE_ORDER__) && defined(__ORDER_BIG_ENDIAN__) && __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__) || \
	    (defined(__BYTE_ORDER) && defined(__BIG_ENDIAN) && __BYTE_ORDER == __BIG_ENDIAN) || \
	    (defined(BYTE_ORDER) && defined(BIG_ENDIAN) && BYTE_ORDER == BIG_ENDIAN)
		#define NAZARA_BIG_ENDIAN
	#elif defined(__i386__) || defined(__i386) || defined(__X86__) || defined (__x86_64) || defined(_M_I86) || \
	    defined(_M_IX86) || defined(_M_X64) || defined(__LITTLE_ENDIAN__) || defined(__ARMEL__) || defined(__THUMBEL__) || \
	    defined(__AARCH64EL__) || defined(_MIPSEL) || defined(__MIPSEL) || defined(__MIPSEL__) || \
	    (defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__) || \
	    (defined(__BYTE_ORDER) && defined(__LITTLE_ENDIAN) && __BYTE_ORDER == __LITTLE_ENDIAN) || \
	    (defined(BYTE_ORDER) && defined(LITTLE_ENDIAN) && BYTE_ORDER == LITTLE_ENDIAN) || \
	    defined(_WIN32) // assume Windows is always LE
		#define NAZARA_LITTLE_ENDIAN
	#else
		#error Failed to identify endianness, you must define either NAZARA_BIG_ENDIAN or NAZARA_LITTLE_ENDIAN
	#endif
#elif defined(NAZARA_BIG_ENDIAN) && defined(NAZARA_LITTLE_ENDIAN)
	#error You cannot define both NAZARA_BIG_ENDIAN and NAZARA_LITTLE_ENDIAN
#endif

namespace Nz
{
	enum class Endianness
	{
		Unknown = -1,

		BigEndian,
		LittleEndian,

		Max = LittleEndian
	};

	constexpr Endianness GetPlatformEndianness();
	inline void SwapBytes(void* buffer, std::size_t size);
	template<typename T> T SwapBytes(T value);
}

#include <NazaraUtils/Endianness.inl>

#endif // NAZARA_UTILS_ENDIANNESS_HPP
