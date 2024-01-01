// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_STRING_HASH_HPP
#define NAZARAUTILS_STRING_HASH_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <string>
#include <string_view>

namespace Nz
{
	template<typename T = char>
	struct StringHash
	{
		using hash_type = std::hash<std::basic_string_view<T>>;
		using is_transparent = void;

		std::size_t operator()(const T* str) const;
		std::size_t operator()(std::basic_string_view<T> str) const;
		template<typename Allocator> std::size_t operator()(const std::basic_string<T, std::char_traits<T>, Allocator>& str) const;
	};
}

#include <NazaraUtils/StringHash.inl>

#endif // NAZARAUTILS_STRING_HASH_HPP
