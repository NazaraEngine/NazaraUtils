// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/DebugBreak.hpp>
#include <cassert>

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
				NazaraDebugBreak();
				assert(false);
			}
		}
	}

	NAZARA_CONSTEXPR20 void Assert(bool condition, std::string_view /*message*/)
	{
		if NAZARA_IS_CONSTEVAL()
		{
			if NAZARA_UNLIKELY(!condition)
				throw AssertionFailed{};
		}
		else
		{
			NazaraDebugBreak();
			assert(false);
		}
	}
}
