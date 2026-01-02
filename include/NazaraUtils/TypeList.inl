// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <type_traits>
#include <utility>

namespace Nz
{
	namespace Detail
	{
		template<typename NewType, typename... ListTypes>
		struct ListAppend<TypeList<ListTypes...>, NewType>
		{
			using Result = TypeList<ListTypes..., NewType>;
		};


		template<template<typename> typename Functor, typename T, typename... ListTypes>
		struct ListApply<TypeList<T, ListTypes...>, Functor>
		{
			template<typename... Args>
			static void Apply(Args&&... args)
			{
				Functor<T>()(std::forward<Args>(args)...);
				if constexpr (sizeof...(ListTypes) > 0)
					ListApply<TypeList<ListTypes...>, Functor>::Apply(std::forward<Args>(args)...);
			}
		};

		template<typename T, typename... ListTypes>
		struct ListAt<TypeList<T, ListTypes...>, 0>
		{
			using Type = T;
		};

		template<std::size_t Index, typename T, typename... ListTypes>
		struct ListAt<TypeList<T, ListTypes...>, Index>
		{
			static_assert(Index <= sizeof...(ListTypes), "Index out of range");

			using Type = typename ListAt<TypeList<ListTypes...>, Index - 1>::Type;
		};


		template<typename... First, typename... Second>
		struct ListConcat<TypeList<First...>, TypeList<Second...>>
		{
			using Result = TypeList<First..., Second...>;
		};


		struct ListFindHelper
		{
			template<std::size_t Index, typename ToFind, typename Type, typename... Rest> static constexpr std::size_t Find()
			{
				if constexpr (std::is_same_v<ToFind, Type>)
					return Index;
				else
					return Find<Index + 1, ToFind, Rest...>();
			}

			template<std::size_t Index, typename ToFind> static constexpr std::size_t Find()
			{
				return std::numeric_limits<std::size_t>::max();
			}
		};

		template<typename TypeToFind, typename... ListTypes>
		struct ListFind<TypeList<ListTypes...>, TypeToFind>
		{
			static constexpr std::size_t Find()
			{
				return ListFindHelper::Find<0, TypeToFind, ListTypes...>();
			}
		};


		template<template<typename...> typename Class, typename... ListTypes>
		struct ListInstantiate<TypeList<ListTypes...>, Class>
		{
			using Result = Class<ListTypes...>;
		};


		template<typename NewType, typename... ListTypes>
		struct ListPrepend<TypeList<ListTypes...>, NewType>
		{
			using Result = TypeList<NewType, ListTypes...>;
		};


		template<typename... Types, typename RemoveType>
		struct ListRebuildWithout<TypeList<Types...>, TypeList<>, RemoveType>
		{
			using Result = TypeList<Types...>;
		};

		template<typename... Types, typename T1, typename... Rest, typename RemoveType>
		struct ListRebuildWithout<TypeList<Types...>, TypeList<T1, Rest...>, RemoveType>
		{
			static constexpr bool CanAddType = !std::is_same_v<T1, RemoveType>;
			using NewList = std::conditional_t<CanAddType, TypeList<Types..., T1>, TypeList<Types...>>;
			using Result = typename ListRebuildWithout<NewList, TypeList<Rest...>, RemoveType>::Result;
		};


		template<typename... ListTypes>
		struct ListSize<TypeList<ListTypes...>>
		{
			static constexpr std::size_t Size = sizeof...(ListTypes);
		};


		template<typename... ListTypes, template<typename> typename Transformer>
		struct ListTransform<TypeList<ListTypes...>, Transformer>
		{
			using Result = TypeList<typename Transformer<ListTypes>::type...>;
		};


		template<typename... Types, typename T1>
		struct ListUnique<TypeList<Types...>, TypeList<T1>>
		{
			static constexpr bool IsTypePresent = TypeListHas<TypeList<Types...>, T1>;
			using Result = std::conditional_t<!IsTypePresent, TypeList<Types..., T1>, TypeList<Types...>>;
		};

		template<>
		struct ListUnique<TypeList<>, TypeList<>>
		{
			using Result = TypeList<>;
		};

		template<typename... Types, typename T1, typename T2, typename... Rest>
		struct ListUnique<TypeList<Types...>, TypeList<T1, T2, Rest...>>
		{
			using Result = typename ListUnique<typename ListUnique<TypeList<Types...>, TypeList<T1>>::Result, TypeList<T2, Rest...>>::Result;
		};
	}

	template<typename List, template<typename> typename Functor, typename... Args>
	void TypeListApply(Args&&... args)
	{
		if constexpr (!TypeListEmpty<List>)
			Detail::ListApply<List, Functor>::Apply(std::forward<Args>(args)...);
	}
}

