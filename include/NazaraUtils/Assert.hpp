// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_ASSERT_HPP
#define NAZARAUTILS_ASSERT_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/ConstantEvaluated.hpp>
#include <string_view>

namespace Nz
{
	struct AssertionFailed {};

	NAZARA_CONSTEXPR20 void Assert(bool condition);
	NAZARA_CONSTEXPR20 void Assert(bool condition, std::string_view message);
}

#include <NazaraUtils/Assert.inl>

#endif // NAZARAUTILS_ASSERT_HPP
