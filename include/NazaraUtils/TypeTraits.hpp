// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_TYPETRAITS_HPP
#define NAZARA_UTILS_TYPETRAITS_HPP

#include <cstddef>
#include <type_traits>

namespace Nz
{
	/************************************************************************/

	template<typename T>
	struct AlwaysFalse : std::false_type {};

	/************************************************************************/

	template<typename T>
	struct LazyVoid
	{
		using type = void;
	};

	template<typename T> using LazyVoid_t = typename LazyVoid<T>::type;

	/************************************************************************/

	template<typename E, typename = void>
	struct EnumValueCount
	{
		static_assert(AlwaysFalse<E>(), "enum has no Count nor Max field");
	};

	template<typename E>
	struct EnumValueCount<E, LazyVoid_t<decltype(E::Count)>>
	{
		static_assert(std::is_enum_v<E>, "Type must be an enumeration");

		static constexpr std::size_t value = static_cast<std::size_t>(E::Count);
	};

	template<typename E>
	struct EnumValueCount<E, LazyVoid_t<decltype(E::Max)>>
	{
		static_assert(std::is_enum_v<E>, "Type must be an enumeration");

		static constexpr std::size_t value = static_cast<std::size_t>(E::Max) + 1;
	};

	template<typename E> constexpr std::size_t EnumValueCount_v = EnumValueCount<E>::value;

	/************************************************************************/

	// Helper for std::visit
	template<typename... Ts> struct Overloaded : Ts...
	{
		using Ts::operator()...;
	};

	template<typename... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

	/************************************************************************/

	template<typename T>
	struct PointedType
	{
		static_assert(AlwaysFalse<T>(), "not a pointer");
	};

	template<typename T> struct PointedType<T*> { using type = T; };
	template<typename T> struct PointedType<T* const> { using type = T; };
	template<typename T> struct PointedType<T* volatile> { using type = T; };
	template<typename T> struct PointedType<T* const volatile> { using type = T; };

	/************************************************************************/

	template<typename T>
	using Pointer = T*;
}

#endif // NAZARA_UTILS_TYPETAG_HPP
