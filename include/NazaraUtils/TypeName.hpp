// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_TYPENAME_HPP
#define NAZARA_UTILS_TYPENAME_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <string_view>

namespace Nz
{
	template<typename T> constexpr std::string_view TypeName();
}

#include <NazaraUtils/TypeName.inl>

#endif // NAZARA_UTILS_TYPENAME_HPP
