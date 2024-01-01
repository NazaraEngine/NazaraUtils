// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_MATHUTILS_HPP
#define NAZARAUTILS_MATHUTILS_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/Algorithm.hpp>
#include <NazaraUtils/ConstantEvaluated.hpp>
#include <NazaraUtils/Constants.hpp>
#include <algorithm>
#include <cmath>

namespace Nz
{
	// Bit/bytes utils
#if defined(NAZARA_COMPILER_MSVC) || defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
	static constexpr bool HasFastFindFirstBit = true;
#else
	static constexpr bool HasFastFindFirstBit = false;
#endif

	template<typename T> constexpr std::size_t BitCount();
	template<typename T> constexpr T ByteSwap(T value);
	template<typename T> constexpr T ClearBit(T number, T bit);
	template<typename T> NAZARA_CONSTEXPR20 std::size_t CountBits(T value);
	template<typename T> NAZARA_CONSTEXPR20 unsigned int FindFirstBit(T number);
	template<typename T> constexpr T ReverseBits(T integer);
	template<typename T> constexpr T SetBit(T number, T bit);
	inline void SwapBytes(void* buffer, std::size_t size);
	template<typename T> constexpr bool TestBit(T number, T bit);
	template<typename T> constexpr T ToggleBit(T number, T bit);

	// Math utils
	template<typename T> constexpr T Align(T offset, T alignment);
	template<typename T> constexpr T AlignPow2(T offset, T alignment);
	template<typename T> constexpr T Approach(T value, T objective, T increment);
	template<typename T> constexpr T Clamp(T value, T min, T max);
	template<typename T> constexpr T DegreeToRadian(T degrees);
	template<typename T> constexpr T GetNearestPowerOfTwo(T number);
	template<typename T> constexpr unsigned int IntegralLog2(T number);
	template<typename T> constexpr unsigned int IntegralLog2Pot(T pot);
	template<typename T> constexpr T IntegralPow(T base, unsigned int exponent);
	template<typename T> constexpr bool IsPow2(T value);
	template<typename T, typename T2> constexpr T Lerp(const T& from, const T& to, const T2& interpolation);
	template<typename T> constexpr T Mod(T x, T y);
	template<typename T> constexpr T MultiplyAdd(T x, T y, T z);
	template<typename T> constexpr bool NumberEquals(T a, T b);
	template<typename T> constexpr bool NumberEquals(T a, T b, T maxDifference);
	template<typename T> constexpr T RadianToDegree(T radians);
	template<typename T> constexpr T RoundToPow2(T value);
}

#include <NazaraUtils/MathUtils.inl>

#endif // NAZARAUTILS_MATHUTILS_HPP
