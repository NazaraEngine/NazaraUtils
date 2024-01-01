// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_TYPENAME_HPP
#define NAZARAUTILS_TYPENAME_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <string_view>

namespace Nz
{
	template<typename T> constexpr std::string_view TypeName();
}

#include <NazaraUtils/TypeName.inl>

#endif // NAZARAUTILS_TYPENAME_HPP
