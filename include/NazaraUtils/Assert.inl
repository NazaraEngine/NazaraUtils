// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/DebugBreak.hpp>
#include <cassert>
#include <cstdio>

namespace Nz
{
	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition)
	{
		if NAZARA_UNLIKELY(!condition)
			AssertFailure();
	}

	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition, const char* message)
	{
		if NAZARA_UNLIKELY(!condition)
			AssertFailure(message);
	}

	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition, const char* file, unsigned int line, const char* message)
	{
		if NAZARA_UNLIKELY(!condition)
			AssertFailure(file, line, message);
	}


	NAZARA_CONSTEXPR20 void AssertFailure(bool dummy)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if (!dummy)
				throw AssertionFailed{};
		}
		else
		{
			std::fputs("assertion failed\n", stderr);
			NazaraDebugBreak();
			assert(false);
		}
	}

	NAZARA_CONSTEXPR20 void AssertFailure(const char* message, bool dummy)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if (!dummy)
				throw AssertionFailed{};
		}
		else
		{
			std::fprintf(stderr, "assertion failed: %s\n", message);
			NazaraDebugBreak();
			assert(false);
		}
	}

	NAZARA_CONSTEXPR20 void AssertFailure(const char* file, unsigned int line, const char* message, bool dummy)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if (!dummy)
				throw AssertionFailed{};
		}
		else
		{
			std::fprintf(stderr, "assertion failed: %s at %s:%d\n", message, file, line);
			NazaraDebugBreak();
			assert(false);
		}
	}
}
