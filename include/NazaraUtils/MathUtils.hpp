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
#include <type_traits>

namespace Nz
{
	// Bit/bytes utils
#if defined(NAZARA_COMPILER_MSVC) || defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
	static constexpr bool HasFastFindFirstBit = true;
#else
	static constexpr bool HasFastFindFirstBit = false;
#endif

	template<typename T1, typename T2> [[nodiscard]] constexpr T1 ArithmeticRightShift(T1 value, T2 shift) noexcept;
	template<typename T> [[nodiscard]] constexpr T ByteSwap(T value) noexcept;
	template<typename T> [[nodiscard]] constexpr T ClearBit(T number, T bit) noexcept;
	template<typename T> [[nodiscard]] NAZARA_CONSTEXPR20 std::size_t CountBits(T value) noexcept;
	template<typename T> [[nodiscard]] NAZARA_CONSTEXPR20 unsigned int FindFirstBit(T number) noexcept;
	template<typename T> [[nodiscard]] constexpr T ReverseBits(T integer) noexcept;
	template<typename T> [[nodiscard]] constexpr T SetBit(T number, T bit) noexcept;
	inline void SwapBytes(void* buffer, std::size_t size) noexcept;
	template<typename T> [[nodiscard]] constexpr bool TestBit(T number, T bit) noexcept;
	template<typename T> [[nodiscard]] constexpr T ToggleBit(T number, T bit) noexcept;

	// Math utils
	template<typename T> [[nodiscard]] constexpr T Align(T offset, T alignment) noexcept;
	template<typename T> [[nodiscard]] constexpr T AlignPow2(T offset, T alignment) noexcept;
	template<typename T> [[nodiscard]] constexpr T Approach(T value, T objective, T increment) noexcept;
	template<typename T> [[nodiscard]] constexpr T Clamp(T value, T min, T max) noexcept;
	template<typename T> [[nodiscard]] constexpr T DegreeToRadian(T degrees) noexcept;
	template<typename T> [[nodiscard]] constexpr T GetNearestPowerOfTwo(T number) noexcept;
	template<typename T> [[nodiscard]] constexpr unsigned int IntegralLog2(T number) noexcept;
	template<typename T> [[nodiscard]] constexpr unsigned int IntegralLog2Pot(T pot) noexcept;
	template<typename T> [[nodiscard]] constexpr T IntegralPow(T base, unsigned int exponent) noexcept;
	template<typename T> [[nodiscard]] constexpr bool IsPow2(T value) noexcept;
	template<typename T, typename T2> [[nodiscard]] constexpr T Lerp(const T& from, const T& to, const T2& interpolation) noexcept;
	template<typename T> [[nodiscard]] constexpr T Mod(T x, T y) noexcept;
	template<typename T> [[nodiscard]] constexpr T MultiplyAdd(T x, T y, T z) noexcept;
	template<typename T> [[nodiscard]] constexpr bool NumberEquals(T a, T b) noexcept;
	template<typename T> [[nodiscard]] constexpr bool NumberEquals(T a, T b, T maxDifference) noexcept;
	template<typename T> [[nodiscard]] constexpr T RadianToDegree(T radians) noexcept;
	template<typename T> [[nodiscard]] constexpr T RoundToPow2(T value) noexcept;
}

#include <NazaraUtils/MathUtils.inl>

#endif // NAZARAUTILS_MATHUTILS_HPP
