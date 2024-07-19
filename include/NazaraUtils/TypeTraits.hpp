// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_TYPETRAITS_HPP
#define NAZARAUTILS_TYPETRAITS_HPP

#include <cstddef>
#include <type_traits>

namespace Nz
{
	/************************************************************************/

	template<typename T>
	struct AlwaysFalse : std::false_type {};

	template<auto T>
	struct AlwaysFalseValue : std::false_type {};

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

	namespace Detail
	{
		template<typename T>
		struct FunctionPtrType;

		template<typename Ret, typename... Args>
		struct FunctionPtrType<Ret(Args...)>
		{
			using type = Ret(*)(Args...);
		};
	}

	template<typename T> using FunctionPtr = typename Detail::FunctionPtrType<T>::type;

	/************************************************************************/

	namespace Detail
	{
		template<typename T>
		struct IsCompleteHelper
		{
			// SFINAE: sizeof in an incomplete type is an error, but since there's another specialization it won't result in a compilation error
			template <typename U>
			static auto test(U*) -> std::bool_constant<sizeof(U) == sizeof(U)>;

			// less specialized overload
			static auto test(...) -> std::false_type;

			using type = decltype(test(static_cast<T*>(nullptr)));
		};
	}

	template <typename T>
	struct IsComplete : Detail::IsCompleteHelper<T>::type {};

	template<typename T>
	constexpr bool IsComplete_v = IsComplete<T>::value;

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

#endif // NAZARAUTILS_TYPETRAITS_HPP
