// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/Algorithm.hpp>
#include <cassert>

#ifdef NAZARA_HAS_CONSTEXPR_BITCAST_STD
#include <bit>
#elif !defined(NAZARA_HAS_CONSTEXPR_BITCAST_BUILTIN)
#include <cstring>
#endif

#ifdef NAZARA_COMPILER_MSVC

#pragma intrinsic(_BitScanForward)

#if defined(NAZARA_ARCH_aarch64) || defined(NAZARA_ARCH_x86_64)

#pragma intrinsic(_BitScanForward64)

#endif

#if defined(NAZARA_ARCH_x86) || defined(NAZARA_ARCH_x86_64)

#include <intrin.h>

#endif

#endif

namespace Nz
{
	namespace Detail
	{
		template<typename From>
		class SafeCaster
		{
			static_assert(std::is_reference_v<From>);

			public:
				template<typename T>
				SafeCaster(T&& from) :
				m_from(std::forward<T>(from))
				{
				}

				template<typename To>
				operator To() const
				{
					return SafeCast<To>(m_from);
				}

			private:
				From m_from;
		};
	}


	/*!
	* \ingroup utils
	* \brief Access a non-typed struct field by offset
	* \return A pointer to the field of the asked type
	*
	* \param basePtr Pointer to the start of the struct
	* \param offset Offset to the field (as generated by offsetof or similar)
	*/
	template<typename T>
	[[nodiscard]] decltype(auto) AccessByOffset(void* basePtr, std::size_t offset) noexcept
	{
		if constexpr (std::is_lvalue_reference_v<T>)
			return *reinterpret_cast<std::remove_reference_t<T>*>(static_cast<UInt8*>(basePtr) + offset);
		else if constexpr (std::is_pointer_v<T>)
			return reinterpret_cast<T>(static_cast<UInt8*>(basePtr) + offset);
		else
			static_assert(AlwaysFalse<T>(), "AccessByOffset requires a reference or pointer type");
	}

	/*!
	* \ingroup utils
	* \brief Access a non-typed struct field by offset
	* \return A pointer to the field of the asked type
	*
	* \param basePtr Pointer to the start of the struct
	* \param offset Offset to the field (as generated by offsetof or similar)
	*/
	template<typename T>
	[[nodiscard]] decltype(auto) AccessByOffset(const void* basePtr, std::size_t offset) noexcept
	{
		static_assert(std::is_lvalue_reference_v<T> || std::is_pointer_v<T>);

		if constexpr (std::is_lvalue_reference_v<T>)
			return *reinterpret_cast<std::remove_reference_t<T>*>(static_cast<const UInt8*>(basePtr) + offset);
		else if constexpr (std::is_pointer_v<T>)
			return reinterpret_cast<T>(static_cast<const UInt8*>(basePtr) + offset);
		else
			static_assert(AlwaysFalse<T>(), "AccessByOffset requires a reference or pointer type");
	}

	template<typename To, typename From>
	[[nodiscard]] NAZARA_CONSTEXPR_BITCAST To BitCast(const From& value) noexcept
	{
		static_assert(sizeof(To) == sizeof(From));
		static_assert(std::is_trivially_copyable_v<From>);
		static_assert(std::is_trivially_copyable_v<To>);

#ifdef NAZARA_HAS_CONSTEXPR_BITCAST_STD
		return std::bit_cast<To>(value);
#elif defined(NAZARA_HAS_CONSTEXPR_BITCAST_BUILTIN)
		return __builtin_bit_cast(To, value);
#else
		To result;
		std::memcpy(&result, &value, sizeof(To));

		return result;
#endif
	}

	/*!
	* \ingroup utils
	* \brief Returns the number of elements in a C-array
	* \return The number of elements
	*
	* \see CountOf
	*/
	template<typename T, std::size_t N>
	[[nodiscard]] constexpr std::size_t CountOf(T(&)[N]) noexcept
	{
		return N;
	}

	/*!
	* \ingroup utils
	* \brief Returns the number of elements in a container
	* \return The number of elements
	*
	* \param c Container with the member function "size()"
	*
	* \see CountOf
	*/
	template<typename T>
	[[nodiscard]] constexpr std::size_t CountOf(const T& c) noexcept
	{
		return c.size();
	}

	template<typename P, typename T>
	[[nodiscard]] NAZARA_CONSTEXPR_BITCAST P IntegerToPointer(T ptrAsInt) noexcept
	{
		static_assert(std::is_pointer_v<P>);

		return BitCast<P>(SafeCast<std::uintptr_t>(ptrAsInt));
	}

	template<typename T, typename P>
	[[nodiscard]] NAZARA_CONSTEXPR_BITCAST T PointerToInteger(P* ptr) noexcept
	{
		return SafeCast<T>(BitCast<std::uintptr_t>(ptr));
	}

	/*!
	* \ingroup utils
	* \brief Helper function to retrieve a key in a map which has to exist
	* \return Value associated with key
	*
	* \param map Map
	* \param key Key, has to exist in map
	*/
	template<typename M, typename T>
	[[nodiscard]] auto& Retrieve(M& map, const T& key) noexcept
	{
		auto it = map.find(key);
		assert(it != map.end());
		return it->second;
	}

	/*!
	* \ingroup utils
	* \brief Helper function to retrieve a key in a map which has to exist
	* \return Value associated with key
	*
	* \param map Map
	* \param key Key, has to exist in map
	*/
	template<typename M, typename T>
	[[nodiscard]] const auto& Retrieve(const M& map, const T& key) noexcept
	{
		auto it = map.find(key);
		assert(it != map.end());
		return it->second;
	}

	template<typename To, typename From>
	[[nodiscard]] To SafeCast(From&& value) noexcept
	{
NAZARA_WARNING_PUSH()
NAZARA_WARNING_MSVC_DISABLE(4702)

#if defined(NAZARA_DEBUG) && !defined(NDEBUG)

		if constexpr (std::is_integral_v<To>)
		{
			if constexpr (std::is_enum_v<From>)
			{
				return SafeCast<To>(static_cast<std::underlying_type_t<From>>(value));
			}
			else if constexpr (std::is_floating_point_v<From>)
			{
				assert(std::floor(value) == value);

				assert(value <= static_cast<From>(std::numeric_limits<To>::max()));
				assert(value >= static_cast<From>(std::numeric_limits<To>::lowest()));
			}
			else if constexpr (std::is_integral_v<From>)
			{
				// Type capable of storing the biggest value between the two types
				using MaxValueType = std::conditional_t<((!std::is_signed_v<From> && std::is_signed_v<To>) || (std::is_signed_v<From> == std::is_signed_v<To> && sizeof(From) > sizeof(To))), From, To>;
				// Type capable of storing the smallest value between the two types
				using MinValueType = std::conditional_t<((std::is_signed_v<From> && !std::is_signed_v<To>) || (std::is_signed_v<From> == std::is_signed_v<To> && sizeof(From) > sizeof(To))), From, To>;

				if constexpr (!std::is_signed_v<To>)
					assert(value >= 0);

				assert(static_cast<MaxValueType>(value) <= static_cast<MaxValueType>(std::numeric_limits<To>::max()));
				assert(static_cast<MinValueType>(value) >= static_cast<MinValueType>(std::numeric_limits<To>::lowest()));
			}
		}
		else if constexpr (std::is_enum_v<To>)
		{
			return static_cast<To>(SafeCast<std::underlying_type_t<To>>(value));
		}
		else if constexpr (std::is_floating_point_v<To>)
		{
			if constexpr (std::is_floating_point_v<From>)
			{
				// Type capable of storing the biggest value between the two types
				using MaxValueType = std::conditional_t<(sizeof(From) > sizeof(To)), From, To>;
				// Type capable of storing the smallest value between the two types
				using MinValueType = std::conditional_t<(sizeof(From) > sizeof(To)), From, To>;

				assert(static_cast<MaxValueType>(value) <= static_cast<MaxValueType>(std::numeric_limits<To>::max()));
				assert(static_cast<MinValueType>(value) >= static_cast<MinValueType>(std::numeric_limits<To>::lowest()));
			}
		}
		else if constexpr (std::is_reference_v<To>)
		{
			if constexpr (std::is_reference_v<From>)
			{
				using BaseFromType = std::remove_reference_t<std::remove_cv_t<From>>;
				using BaseToType = std::remove_reference_t<std::remove_cv_t<To>>;

				if constexpr (!std::is_same_v<BaseFromType, BaseToType> && std::is_base_of_v<From, To> && std::is_polymorphic_v<From>)
				{
					using ToPtr = std::add_pointer_t<std::remove_reference_t<To>>;
					assert(dynamic_cast<ToPtr>(&value) != nullptr);
				}
			}
		}
		else if constexpr (std::is_pointer_v<To>)
		{
			if constexpr (std::is_pointer_v<From>)
			{
				using BaseFromType = std::remove_pointer_t<std::remove_cv_t<From>>;
				using BaseToType = std::remove_pointer_t<std::remove_cv_t<To>>;

				if constexpr (!std::is_same_v<BaseFromType, BaseToType> && std::is_base_of_v<From, To> && std::is_polymorphic_v<From>)
				{
					assert(!value || dynamic_cast<To>(value) != nullptr);
				}
			}
		}

#endif

		return static_cast<To>(value);

NAZARA_WARNING_POP()
	}

	template<typename From>
	[[nodiscard]] auto SafeCaster(From&& value) noexcept
	{
		return Detail::SafeCaster<decltype(value)>(std::forward<From>(value));
	}

	template<typename T, typename U>
	[[nodiscard]] std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& ptr) noexcept
	{
		return std::unique_ptr<T>(SafeCast<T*>(ptr.release()));
	}

	template<typename T>
	[[nodiscard]] constexpr auto UnderlyingCast(T value) noexcept -> std::underlying_type_t<T>
	{
		return static_cast<std::underlying_type_t<T>>(value);
	}
}

