// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_FLAGS_HPP
#define NAZARAUTILS_FLAGS_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/MathUtils.hpp>
#include <NazaraUtils/TypeTraits.hpp>
#include <iterator>
#include <type_traits>

namespace Nz
{
	// From: https://www.justsoftwaresolutions.co.uk/cplusplus/using-enum-classes-as-bitfields.html
	template<typename E>
	struct EnumAsFlags;

	// You can implement this in the same namespace as the enum thanks to ADL
	template<typename T> constexpr bool EnableEnumAsNzFlags(T) { return false; };
	template<typename T> constexpr bool EnableAutoFlagForNzFlags(T) { return true; };

	template<typename T, typename = void>
	struct IsEnumFlag : std::bool_constant<IsComplete_v<EnumAsFlags<T>>> {};

	template<typename T>
	struct IsEnumFlag<T, std::void_t<std::enable_if_t<EnableEnumAsNzFlags(T{})>>> : std::true_type {};

	template<typename E, typename = void>
	struct GetEnumAutoFlag : std::bool_constant<EnableAutoFlagForNzFlags(E{})> {};

	template<typename E>
	struct GetEnumAutoFlag<E, std::void_t<decltype(E::AutoFlag)>> : std::bool_constant<E::AutoFlag> {};

	template<typename E, typename = void>
	struct GetEnumFlagMax
	{
		static constexpr std::size_t max = EnumValueCount<E>::value - 1;
	};

	template<typename E>
	struct GetEnumFlagMax<E, std::void_t<decltype(EnumAsFlags<E>::max)>>
	{
		static constexpr std::size_t max = static_cast<std::size_t>(EnumAsFlags<E>::max);
	};

	template<typename E>
	class Flags
	{
		static_assert(std::is_enum_v<E>, "Type must be an enumeration");
		static_assert(IsEnumFlag<E>(), "Enum has not been enabled as flags by an EnumAsFlags specialization nor EnableEnumAsNzFlags ADL function");

		static constexpr std::size_t MaxValue = GetEnumFlagMax<E>::max;
		static constexpr bool AutoFlag = GetEnumAutoFlag<E>();

		using BitField16 = std::conditional_t<(MaxValue >= 8), UInt16, UInt8>;
		using BitField32 = std::conditional_t<(MaxValue >= 16), UInt32, BitField16>;

		public:
			class iterator;
			friend iterator;

			using BitField = std::conditional_t<(MaxValue >= 32), UInt64, BitField32>;

			constexpr Flags(BitField value = 0);
			constexpr Flags(E enumVal);

			constexpr void Clear();
			constexpr void Clear(const Flags& flags);
			std::size_t Count() const;

			constexpr void Set(const Flags& flags);

			constexpr bool Test(const Flags& flags) const;

			constexpr iterator begin() const;
			constexpr iterator end() const;
			std::size_t size() const;

			explicit constexpr operator bool() const;
			template<typename T, typename = std::enable_if_t<std::is_integral<T>::value && sizeof(T) >= sizeof(BitField)>> explicit constexpr operator T() const;

			constexpr Flags operator~() const;
			constexpr Flags operator&(const Flags& rhs) const;
			constexpr Flags operator|(const Flags& rhs) const;
			constexpr Flags operator^(const Flags& rhs) const;

			constexpr bool operator==(const Flags& rhs) const;
			constexpr bool operator!=(const Flags& rhs) const;

			constexpr Flags& operator|=(const Flags& rhs);
			constexpr Flags& operator&=(const Flags& rhs);
			constexpr Flags& operator^=(const Flags& rhs);

			static constexpr BitField GetFlagValue(E enumValue);

			static constexpr BitField ValueMask = BitField((UInt64(1) << (MaxValue + 1)) - 1);

		private:
			BitField m_value;
	};

	template<typename T>
	class Flags<T>::iterator
	{
		friend Flags;

		public:
			using iterator_category = std::input_iterator_tag;
			using value_type = T;
			using difference_type = std::ptrdiff_t;

			iterator(const iterator&) = default;
			iterator(iterator&&) = default;

			iterator& operator=(const iterator&) = default;
			iterator& operator=(iterator&&) = default;

			iterator operator++(int);
			iterator& operator++();

			constexpr bool operator==(const iterator& rhs) const;
			constexpr bool operator!=(const iterator& rhs) const;
			value_type operator*() const;

		private:
			constexpr iterator(BitField remainingFlags);

			BitField m_remainingFlags;
	};

	template<typename E> constexpr Flags<E> operator&(E lhs, Flags<E> rhs);
	template<typename E> constexpr Flags<E> operator|(E lhs, Flags<E> rhs);
	template<typename E> constexpr Flags<E> operator^(E lhs, Flags<E> rhs);

	// Little hack to have them in both Nz and global scope
	namespace FlagsOperators
	{
		template<typename E> constexpr std::enable_if_t<IsEnumFlag<E>::value, Flags<E>> operator~(E lhs);
		template<typename E> constexpr std::enable_if_t<IsEnumFlag<E>::value, Flags<E>> operator&(E lhs, E rhs);
		template<typename E> constexpr std::enable_if_t<IsEnumFlag<E>::value, Flags<E>> operator|(E lhs, E rhs);
		template<typename E> constexpr std::enable_if_t<IsEnumFlag<E>::value, Flags<E>> operator^(E lhs, E rhs);
	}

	using namespace FlagsOperators;
}

using namespace Nz::FlagsOperators;

#include <NazaraUtils/Flags.inl>

#endif // NAZARAUTILS_FLAGS_HPP
