// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_ASSERT_HPP
#define NAZARAUTILS_ASSERT_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/ConstantEvaluated.hpp>

#if !defined(NAZARA_NO_ASSERT) && !defined(NDEBUG)
	#define NazaraAssert(cond) if NAZARA_UNLIKELY(!(cond)) Nz::AssertFailureWithSource(__FILE__, __LINE__, #cond)
	#define NazaraAssertMsg(cond, ...) if NAZARA_UNLIKELY(!(cond)) Nz::AssertFailureWithSource(__FILE__, __LINE__, __VA_ARGS__)
#else
	#define NazaraAssert(...) do {} while (false)
	#define NazaraAssertMsg(...) do {} while (false)
#endif

namespace Nz
{
	struct AssertionFailed {};

	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition);
	template<typename... Args> NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition, const char* message, Args&&... args);
	template<typename... Args> NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void AssertWithSource(bool condition, const char* file, unsigned int line, const char* message, Args&&... args);

	template<bool Dummy = false> NAZARA_CONSTEXPR20 void AssertFailure();
	template<bool Dummy = false, typename... Args> NAZARA_CONSTEXPR20 void AssertFailure(const char* message, Args&&... args);
	template<bool Dummy = false, typename... Args> NAZARA_CONSTEXPR20 void AssertFailureWithSource(const char* file, unsigned int line, const char* message, Args&&... args);
}

#include <NazaraUtils/Assert.inl>

#endif // NAZARAUTILS_ASSERT_HPP
