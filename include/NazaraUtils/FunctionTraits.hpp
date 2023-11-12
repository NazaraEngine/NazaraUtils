// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_FUNCTIONREF_HPP
#define NAZARA_UTILS_FUNCTIONREF_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/TypeList.hpp>
#include <type_traits>

namespace Nz
{
	template<typename T>
	struct FunctionTraits : FunctionTraits<decltype(&T::operator())>
	{
	};

	template<typename Ret, typename... Args>
	struct FunctionTraits<Ret(Args...)>
	{
		using FuncType = Ret(Args...);
		using ReturnType = Ret;

		using ArgTypes = TypeList<Args...>;
		static constexpr std::size_t ArgCount = sizeof...(Args);

		template<std::size_t I>
		struct Arg
		{
			static_assert(I < ArgCount, "argument out of range");
			using type = TypeListAt<ArgTypes, I>;
		};

		template<std::size_t I>
		using ArgType = typename Arg<I>::type;
	};

	template<typename Ret, typename... Args>
	struct FunctionTraits<Ret(*)(Args...)> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...)> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) &> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) &&> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) const> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) const&> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) const&&> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) volatile> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) volatile&> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) volatile&&> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) const volatile> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) const volatile&> : FunctionTraits<Ret(Args...)> {};

	template<typename T, typename Ret, typename... Args>
	struct FunctionTraits<Ret(T::*)(Args...) const volatile&&> : FunctionTraits<Ret(Args...)> {};

	template<typename T>
	struct FunctionTraits<const T> : FunctionTraits<T> {};

	template<typename T>
	struct FunctionTraits<const T&> : FunctionTraits<T> {};

	template<typename T>
	struct FunctionTraits<const T&&> : FunctionTraits<T> {};

	template<typename T>
	struct FunctionTraits<T&> : FunctionTraits<T> {};

	template<typename T>
	struct FunctionTraits<T&&> : FunctionTraits<T> {};

	/************************************************************************/

	template<typename, typename = void>
	struct IsFunctor : std::false_type {};

	template<typename T>
	struct IsFunctor<T, std::enable_if_t<std::is_function_v<T>>> : std::true_type {};

	template<typename T>
	struct IsFunctor<T, std::enable_if_t<std::is_member_function_pointer_v<T>>> : std::true_type {};

	template<typename T>
	struct IsFunctor<T, std::void_t<decltype(&T::operator())>> : std::true_type {};

	template<typename T>
	constexpr bool IsFunctor_v = IsFunctor<T>::value;

	/************************************************************************/

	template<typename... Args>
	struct OverloadResolver
	{
		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...)) const noexcept
		{
			return ptr;
		}

		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...)&) const noexcept
		{
			return ptr;
		}

		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...)&&) const noexcept
		{
			return ptr;
		}

		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...) const) const noexcept
		{
			return ptr;
		}

		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...) const&) const noexcept
		{
			return ptr;
		}

		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...) const&&) const noexcept
		{
			return ptr;
		}

		template<typename R>
		constexpr auto operator()(R(*ptr)(Args...)) const noexcept
		{
			return ptr;
		}
	};

	template<typename... Args> constexpr OverloadResolver<Args...> Overload = {};

}

#endif // NAZARA_UTILS_FUNCTIONREF_HPP
