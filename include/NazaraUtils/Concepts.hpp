// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_COONCEPTS_HPP
#define NAZARAUTILS_COONCEPTS_HPP

#include <NazaraUtils/Prerequisites.hpp>

#ifdef __cpp_concepts
	#define NAZARA_HAS_CONCEPTS

	#if __has_include(<version>)
		#include <version>
	#endif

	#define NAZARA_CONCEPT(arg) arg
	#define NAZARA_CONCEPT_T(arg) arg
#else
	#define NAZARA_CONCEPT(arg)
	#define NAZARA_CONCEPT_T(arg) typename
#endif

#if defined(NAZARA_HAS_CONCEPTS) && __cpp_lib_concepts
	#include <concepts>
	#define NAZARA_HAS_STD_CONCEPTS
	#define NAZARA_STD_CONCEPT_T(arg) arg
#else
	#define NAZARA_STD_CONCEPT_T(arg) typename
#endif

namespace Nz
{
#ifdef NAZARA_HAS_STD_CONCEPTS
	template<typename T> concept Arithmetic = std::is_arithmetic_v<T>;
#endif
}

#endif // NAZARAUTILS_COONCEPTS_HPP
