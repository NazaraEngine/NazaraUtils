// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/StringHash.hpp>

namespace Nz
{
	template<typename T>
	std::size_t StringHash<T>::operator()(const T* str) const
	{
		return hash_type{}(str);
	}

	template<typename T>
	std::size_t StringHash<T>::operator()(std::basic_string_view<T> str) const
	{
		return hash_type{}(str);
	}
	
	template<typename T>
	template<typename Allocator>
	std::size_t StringHash<T>::operator()(const std::basic_string<T, std::char_traits<T>, Allocator>& str) const
	{
		return hash_type{}(str);
	}
}

