// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_PATH_HASH_HPP
#define NAZARAUTILS_PATH_HASH_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/StringUtils.hpp>
#include <filesystem>
#include <string>

namespace Nz
{
	// Path utils
	inline [[nodiscard]] std::string PathToString(const std::filesystem::path& path);
	inline [[nodiscard]] std::filesystem::path Utf8Path(std::string_view path);
}

#include <NazaraUtils/PathUtils.inl>

#endif // NAZARAUTILS_PATH_HASH_HPP
