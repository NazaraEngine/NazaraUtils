// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_CONSTANTS_HPP
#define NAZARAUTILS_CONSTANTS_HPP

#include <NazaraUtils/Prerequisites.hpp>

namespace Nz
{
	// Math constants
	template<typename T> constexpr T HalfPi = T(1.5707963267948966192313216916398);
	template<typename T> constexpr T Pi     = T(3.1415926535897932384626433832795);
	template<typename T> constexpr T Sqrt2  = T(1.4142135623730950488016887242097);
	template<typename T> constexpr T Sqrt3  = T(1.7320508075688772935274463415059);
	template<typename T> constexpr T Sqrt5  = T(2.2360679774997896964091736687313);
	template<typename T> constexpr T Tau    = T(6.2831853071795864769252867665590);
}

#endif // NAZARAUTILS_CONSTANTS_HPP
