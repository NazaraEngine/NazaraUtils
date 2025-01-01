// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/DebugBreak.hpp>
#include <cassert>
#include <cstdio>
#include <cstdlib>
#include <utility> // std::forward

namespace Nz
{
	template<bool Dummy>
	NAZARA_CONSTEXPR20 void AssertFailure()
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if constexpr (!Dummy) //< just to make the throw dependent
				throw AssertionFailed{};
		}
		else
		{
			std::fputs("assertion failed\n", stderr);
			NazaraDebugBreak();
			assert(false);
			std::abort();
		}
	}

	template<bool Dummy, typename... Args>
	NAZARA_CONSTEXPR20 void AssertFailure(const char* message, Args&&... args)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if constexpr (!Dummy) //< just to make the throw dependent
				throw AssertionFailed{};
		}
		else
		{
			if constexpr (sizeof...(Args) > 0)
			{
				std::fputs("assertion failed: ", stderr);
				std::fprintf(stderr, message, std::forward<Args>(args)...);
				std::fputc('\n', stderr);
			}
			else
			{
				std::fprintf(stderr, "assertion failed: %s\n", message);
			}

			NazaraDebugBreak();
			assert(false);
			std::abort();
		}
	}

	template<bool Dummy, typename... Args>
	NAZARA_CONSTEXPR20 void AssertFailureWithSource(const char* file, unsigned int line, const char* message, Args&&... args)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if constexpr (!Dummy) //< just to make the throw dependent
				throw AssertionFailed{};
		}
		else
		{
			if constexpr (sizeof...(Args) > 0)
			{
				std::fputs("assertion failed: ", stderr);
				std::fprintf(stderr, message, std::forward<Args>(args)...);
				std::fprintf(stderr, " at %s:%d: ", file, line);
				std::fputc('\n', stderr);
			}
			else
			{
				std::fprintf(stderr, "assertion failed: %s at %s:%d\n", message, file, line);
			}
			NazaraDebugBreak();
			assert(false);
			std::abort();
		}
	}

	// Defined after AssertFailure because otherwise GCC warns about undefined inline
	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition)
	{
		if NAZARA_UNLIKELY(!condition)
			AssertFailure();
	}

	template<typename... Args>
	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void Assert(bool condition, const char* message, Args&&... args)
	{
		if NAZARA_UNLIKELY(!condition)
			AssertFailure(message, std::forward<Args>(args)...);
	}

	template<typename... Args>
	NAZARA_CONSTEXPR20 NAZARA_FORCEINLINE void AssertWithSource(bool condition, const char* file, unsigned int line, const char* message, Args&&... args)
	{
		if NAZARA_UNLIKELY(!condition)
			AssertFailureWithSource(file, line, message, std::forward<Args>(args)...);
	}
}
