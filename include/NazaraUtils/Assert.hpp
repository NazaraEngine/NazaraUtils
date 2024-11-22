// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_ASSERT_HPP
#define NAZARAUTILS_ASSERT_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/ConstantEvaluated.hpp>
#include <string_view>

#if !defined(NAZARA_NO_ASSERT) && !defined(NDEBUG)
	#define NazaraAssert(cond, ...) if NAZARA_UNLIKELY(!(cond)) Nz::AssertFailure(__FILE__, __LINE__, #cond __VA_ARGS__)
#else
	#define NazaraAssert(cond, ...) do {} while (false)
#endif

namespace Nz
{
	struct AssertionFailed {};

	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition);
	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition, const char* message);
	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition, const char* file, unsigned int line, const char* message);

	NAZARA_CONSTEXPR20 void AssertFailure(bool dummy = false);
	NAZARA_CONSTEXPR20 void AssertFailure(const char* message, bool dummy = false);
	NAZARA_CONSTEXPR20 void AssertFailure(const char* file, unsigned int line, const char* message, bool dummy = false);
}

#include <NazaraUtils/Assert.inl>

#endif // NAZARAUTILS_ASSERT_HPP
