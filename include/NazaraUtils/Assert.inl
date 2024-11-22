// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/DebugBreak.hpp>
#include <cassert>
#include <cstdio>

namespace Nz
{
	NAZARA_CONSTEXPR20 void Assert(bool condition)
	{
		if NAZARA_UNLIKELY(!condition)
		{
			if NAZARA_IS_CONSTEVAL()
			{
				throw AssertionFailed{};
			}
			else
			{
				std::fputs("assertion failed\n", stderr);
				NazaraDebugBreak();
				assert(false);
			}
		}
	}

	NAZARA_CONSTEXPR20 void Assert(bool condition, const char* message)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if NAZARA_UNLIKELY(!condition)
				throw AssertionFailed{};
		}
		else
		{
			if NAZARA_UNLIKELY(!condition)
			{
				std::fprintf(stderr, "assertion failed: %s\n", message);
				NazaraDebugBreak();
				assert(false);
			}
		}
	}

	NAZARA_CONSTEXPR20 void Assert(bool condition, const char* file, unsigned int line, const char* message)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if NAZARA_UNLIKELY(!condition)
				throw AssertionFailed{};
		}
		else
		{
			if NAZARA_UNLIKELY(!condition)
			{
				std::fprintf(stderr, "assertion failed: %s at %s:%d\n", message, file, line);
				NazaraDebugBreak();
				assert(false);
			}
		}
	}
}
