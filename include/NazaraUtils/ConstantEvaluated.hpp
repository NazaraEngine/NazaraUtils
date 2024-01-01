// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_CONSTANTEVALUATED_HPP
#define NAZARAUTILS_CONSTANTEVALUATED_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <type_traits>

/*
* Uncomment when C++23 is out
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP23)
	#define NAZARA_HAS_CONSTEVAL
	#define NAZARA_CONSTEXPR20 constexpr
	#define NAZARA_IS_CONSTEVAL() consteval
	#define NAZARA_IS_NOT_CONSTEVAL() !consteval
*/
#if NAZARA_CHECK_CPP_VER(NAZARA_CPP20)
	#define NAZARA_HAS_CONSTEVAL
	#define NAZARA_CONSTEXPR20 constexpr
	#define NAZARA_IS_CONSTEVAL() (std::is_constant_evaluated())
	#define NAZARA_IS_RUNTIME_EVAL() (!std::is_constant_evaluated())
#elif NAZARA_CHECK_MSVC_VER(1925) || NAZARA_CHECK_CLANG_VER(900) || NAZARA_CHECK_GCC_VER(900)
	// Supported through compiler extension
	#define NAZARA_HAS_CONSTEVAL
	#define NAZARA_CONSTEXPR20 constexpr
	#define NAZARA_IS_CONSTEVAL() (__builtin_is_constant_evaluated())
	#define NAZARA_IS_RUNTIME_EVAL() (!__builtin_is_constant_evaluated())
#else
	#define NAZARA_CONSTEXPR20
	#define NAZARA_IS_CONSTEVAL() (false)
	#define NAZARA_IS_RUNTIME_EVAL() (true)
#endif

#endif // NAZARAUTILS_CONSTANTEVALUATED_HPP