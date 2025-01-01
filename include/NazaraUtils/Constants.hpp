// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_CONSTANTS_HPP
#define NAZARAUTILS_CONSTANTS_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/Concepts.hpp>
#include <climits>
#include <limits>

namespace Nz
{
	template<typename T> constexpr std::size_t BitCount = CHAR_BIT * sizeof(T);

	// Make a class so it can be specialized (T = void) to return casters
	template<typename T>
	NAZARA_CONCEPT(requires std::is_arithmetic_v<T> || std::is_void_v<T>)
	struct MathConstants
	{
		static constexpr T Infinity()
		{
			static_assert(std::numeric_limits<T>::has_infinity);
			return std::numeric_limits<T>::infinity();
		}

		static constexpr T Max()
		{
			return std::numeric_limits<T>::max();
		}

		static constexpr T Min()
		{
			return std::numeric_limits<T>::min();
		}

		static constexpr T NaN()
		{
			static_assert(std::numeric_limits<T>::has_quiet_NaN);
			return std::numeric_limits<T>::quiet_NaN();
		}

		// Math constants
		static constexpr T HalfPi()
		{
			static_assert(std::is_floating_point_v<T>);
			return T(1.5707963267948966192313216916398);
		}

		static constexpr T Pi()
		{
			static_assert(std::is_floating_point_v<T>);
			return T(3.1415926535897932384626433832795);
		}

		static constexpr T Sqrt2()
		{
			static_assert(std::is_floating_point_v<T>);
			return T(1.4142135623730950488016887242097);
		}

		static constexpr T Sqrt3()
		{
			static_assert(std::is_floating_point_v<T>);
			return T(1.7320508075688772935274463415059);
		}

		static constexpr T Sqrt5()
		{
			static_assert(std::is_floating_point_v<T>);
			return T(2.2360679774997896964091736687313);
		}

		static constexpr T Tau()
		{
			static_assert(std::is_floating_point_v<T>);
			return T(6.2831853071795864769252867665590);
		}
	};

	// Type constants
	template<typename T = void> constexpr auto Infinity() { return MathConstants<T>::Infinity(); }
	template<typename T = void> constexpr auto MaxValue() { return MathConstants<T>::Max(); }
	template<typename T = void> constexpr auto MinValue() { return MathConstants<T>::Min(); }
	template<typename T = void> constexpr auto NaN()      { return MathConstants<T>::NaN(); }

	// Math constants
	template<typename T = void> constexpr auto HalfPi() { return MathConstants<T>::HalfPi(); }
	template<typename T = void> constexpr auto Pi()     { return MathConstants<T>::Pi(); }
	template<typename T = void> constexpr auto Sqrt2()  { return MathConstants<T>::Sqrt2(); }
	template<typename T = void> constexpr auto Sqrt3()  { return MathConstants<T>::Sqrt3(); }
	template<typename T = void> constexpr auto Sqrt5()  { return MathConstants<T>::Sqrt5(); }
	template<typename T = void> constexpr auto Tau()    { return MathConstants<T>::Tau(); }
}

#include <NazaraUtils/Constants.inl>

#endif // NAZARAUTILS_CONSTANTS_HPP
