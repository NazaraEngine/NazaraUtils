// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_MEMORYHELPER_HPP
#define NAZARAUTILS_MEMORYHELPER_HPP

#include <NazaraUtils/Prerequisites.hpp>

#if defined(NAZARA_PLATFORM_WINDOWS) // MSVC / MinGW / Clang on Windows

#include <malloc.h>

// with MSVC, using alloca with a size of zero returns a valid pointer
#define NAZARA_ALLOCA(size) _alloca(size)
#define NAZARA_ALLOCA_SUPPORT

#elif defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC) || defined(NAZARA_COMPILER_INTEL)

#ifdef NAZARA_PLATFORM_BSD
	#include <stdlib.h>
#else
	#include <alloca.h>
#endif

// with Clang/GCC, using alloca with a size of zero does nothing good
#define NAZARA_ALLOCA(size) alloca(((size) > 0) ? (size) : 1)
#define NAZARA_ALLOCA_SUPPORT

#endif

#include <cstddef>

namespace Nz
{
	template<typename T, typename... Args>
	constexpr T* PlacementNew(T* ptr, Args&&... args);

	template<typename T>
	constexpr void PlacementDestroy(T* ptr);
}

#include <NazaraUtils/MemoryHelper.inl>

#endif // NAZARAUTILS_MEMORYHELPER_HPP
