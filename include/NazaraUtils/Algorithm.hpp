// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_ALGORITHM_HPP
#define NAZARA_UTILS_ALGORITHM_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/TypeTag.hpp>
#include <filesystem>
#include <functional>
#include <memory>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <unordered_map>
#include <utility>

// is_constant_evaluated support
/*
* Uncomment when C++23 is out
#if NAZARA_CPP_VER >= NAZARA_CPP23
	#define NAZARA_HAS_CONSTEVAL
	#define NAZARA_CONSTEXPR20 constexpr
	#define NAZARA_IS_CONSTEVAL() consteval
	#define NAZARA_IS_NOT_CONSTEVAL() !consteval
*/
#if NAZARA_CPP_VER >= NAZARA_CPP20
	#define NAZARA_HAS_CONSTEVAL
	#define NAZARA_CONSTEXPR20 constexpr
	#define NAZARA_IS_CONSTEVAL() (std::is_constant_evaluated())
	#define NAZARA_IS_RUNTIME_EVAL() (!std::is_constant_evaluated())
#elif NAZARA_CHECK_MSVC_VER(1925) || NAZARA_CHECK_CLANG_VER(900) || NAZARA_CHECK_GCC_VER(900)
	// Supported through compiler extension
	#define NAZARA_HAS_CONSTEVAL
	#define NAZARA_CONSTEXPR20 constexpr
	#define NAZARA_IS_CONSTEVAL() (__builtin_is_constant_evaluated())
	#define NAZARA_IS_RUNTIME_EVAL() (!__builtin_is_constant_evaluated())
#else
	#define NAZARA_CONSTEXPR20
	#define NAZARA_IS_CONSTEVAL() (false)
	#define NAZARA_IS_RUNTIME_EVAL() (true)
#endif

namespace Nz
{
	// Math constants
	template<typename T> constexpr T HalfPi = T(1.5707963267948966192313216916398);
	template<typename T> constexpr T Pi     = T(3.1415926535897932384626433832795);
	template<typename T> constexpr T Sqrt2  = T(1.4142135623730950488016887242097);
	template<typename T> constexpr T Sqrt3  = T(1.7320508075688772935274463415059);
	template<typename T> constexpr T Sqrt5  = T(2.2360679774997896964091736687313);
	template<typename T> constexpr T Tau    = T(6.2831853071795864769252867665590);


	// Math utils
#if defined(NAZARA_COMPILER_MSVC) || defined(NAZARA_COMPILER_CLANG) || defined(NAZARA_COMPILER_GCC)
	static constexpr bool HasFastFindFirstBit = true;
#else
	static constexpr bool HasFastFindFirstBit = false;
#endif

	template<typename T> constexpr T Approach(T value, T objective, T increment);
	template<typename T> constexpr T Clamp(T value, T min, T max);
	template<typename T> constexpr T ClearBit(T number, T bit);
	template<typename T> NAZARA_CONSTEXPR20 std::size_t CountBits(T value);
	template<typename T> constexpr T DegreeToRadian(T degrees);
	template<typename T> NAZARA_CONSTEXPR20 unsigned int FindFirstBit(T number);
	template<typename T> constexpr T GetNearestPowerOfTwo(T number);
	template<typename T> constexpr unsigned int IntegralLog2(T number);
	template<typename T> constexpr unsigned int IntegralLog2Pot(T pot);
	template<typename T> constexpr T IntegralPow(T base, unsigned int exponent);
	template<typename T, typename T2> constexpr T Lerp(const T& from, const T& to, const T2& interpolation);
	template<typename T> constexpr T MultiplyAdd(T x, T y, T z);
	template<typename T> constexpr bool NumberEquals(T a, T b);
	template<typename T> constexpr bool NumberEquals(T a, T b, T maxDifference);
	template<typename T> constexpr T RadianToDegree(T radians);
	template<typename T> constexpr T SetBit(T number, T bit);
	template<typename T> constexpr bool TestBit(T number, T bit);
	template<typename T> constexpr T ToggleBit(T number, T bit);

	// Path utils
	inline std::string PathToString(const std::filesystem::path& path);
	inline std::filesystem::path Utf8Path(std::string_view path);

	// String utils
#if NAZARA_CPP_VER >= NAZARA_CPP20 && !defined(NAZARA_PLATFORM_ANDROID)
	inline std::string FromUtf8String(const std::u8string& str);
	inline std::string_view FromUtf8String(std::u8string_view str);
	inline std::string_view FromUtf8String(const char8_t* str);
#else
	inline std::string FromUtf8String(std::string str);
	inline std::string_view FromUtf8String(std::string_view str);
#endif

#if NAZARA_CPP_VER >= NAZARA_CPP20 && !defined(NAZARA_PLATFORM_ANDROID)
	inline std::u8string ToUtf8String(const std::string& str);
	inline std::u8string_view ToUtf8String(std::string_view str);
	inline std::u8string_view ToUtf8String(const char* str);
#else
	inline std::string ToUtf8String(std::string str);
	inline std::string_view ToUtf8String(std::string_view str);
#endif

	// Generic utils
	template<typename T> decltype(auto) AccessByOffset(void* basePtr, std::size_t offset);
	template<typename T> decltype(auto) AccessByOffset(const void* basePtr, std::size_t offset);
	template<typename T> constexpr T Align(T offset, T alignment);
	template<typename T> constexpr T AlignPow2(T offset, T alignment);
	template<typename T> constexpr std::size_t BitCount();
	template<typename T, std::size_t N> constexpr std::size_t CountOf(T(&name)[N]) noexcept;
	template<typename T> std::size_t CountOf(const T& c);
	template<std::size_t N> constexpr std::size_t CountOf(const char(&str)[N]) noexcept;
	template<typename T> constexpr bool IsPowerOfTwo(T value);
	template<typename M, typename T> auto& Retrieve(M& map, const T& key);
	template<typename M, typename T> const auto& Retrieve(const M& map, const T& key);
	template<typename T> constexpr T ReverseBits(T integer);
	template<typename To, typename From> To SafeCast(From&& value);
	template<typename T, typename U> std::unique_ptr<T> StaticUniquePointerCast(std::unique_ptr<U>&& ptr);
	template<typename T> constexpr auto UnderlyingCast(T value) -> std::underlying_type_t<T>;

	template<typename T>
	struct AlwaysFalse : std::false_type {};

	template<typename T>
	struct LazyVoid
	{
		using type = void;
	};

	template<typename T> using LazyVoid_t = typename LazyVoid<T>::type;

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

	// Helper for std::visit
	template<typename... Ts> struct Overloaded : Ts...
	{
		using Ts::operator()...;
	};

	template<typename... Ts> Overloaded(Ts...) -> Overloaded<Ts...>;

	template<typename... Args>
	struct OverloadResolver
	{
		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...)) const noexcept
		{
			return ptr;
		}

		template<typename R, typename T>
		constexpr auto operator()(R(T::* ptr)(Args...) const) const noexcept
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

	template<typename T>
	struct PointedType
	{
		using type = void; //< FIXME: I can't make SFINAE work
	};

	template<typename T>
	using Pointer = T*;
}

#include <NazaraUtils/Algorithm.inl>

#endif // NAZARA_UTILS_ALGORITHM_HPP
