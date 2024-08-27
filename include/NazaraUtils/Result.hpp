// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_RESULT_HPP
#define NAZARAUTILS_RESULT_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <optional>
#include <string>
#include <string_view>
#include <type_traits>
#include <variant>

#define NAZARA_TRY(expr) \
	do \
	{  \
		auto NazaraSuffixMacro(result_, __LINE__) = (expr); \
		if NAZARA_UNLIKELY(!NazaraSuffixMacro(result_, __LINE__).IsOk()) \
			return Nz::Err(std::move(NazaraSuffixMacro(result_, __LINE__)).GetError()); \
	} \
	while (false)

#define NAZARA_TRY_VALUE(var, expr) \
	auto NazaraSuffixMacro(result_, __LINE__) = (expr); \
	if NAZARA_UNLIKELY(!NazaraSuffixMacro(result_, __LINE__).IsOk()) \
		return Nz::Err(std::move(NazaraSuffixMacro(result_, __LINE__)).GetError()); \
	var = std::move(NazaraSuffixMacro(result_, __LINE__)).GetValue()

namespace Nz
{
	template<typename V>
	struct ResultValue
	{
		V value;
	};

	template<>
	struct ResultValue<void>
	{
	};


	template<typename E>
	struct ResultError
	{
		E value;
	};


	constexpr ResultValue<void> Ok();
	template<typename V> constexpr auto Ok(V&& value);
	template<typename E> constexpr auto Err(E&& err);

	template<typename V, typename E>
	class Result
	{
		static_assert(!std::is_void_v<E>, "error type cannot be void");

		public:
			struct ValueTag {};
			struct ErrorTag {};

			template<typename T2 = V, typename = std::enable_if_t<std::is_default_constructible_v<T2>>> constexpr Result() {};
			template<typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, V> && !std::is_same_v<V, E>>> constexpr Result(T&& value);
			template<typename T> constexpr Result(ResultValue<T> value);
			template<typename T> constexpr Result(ResultError<T> error);
			template<typename... Args> constexpr Result(ValueTag, Args&&... args);
			template<typename... Args> constexpr Result(ErrorTag, Args&&... args);
			template<typename V2, typename E2, typename = std::enable_if_t<std::is_convertible_v<V2, V> && std::is_convertible_v<E2, E>>> constexpr Result(const Result<V2, E2>& result);
			template<typename V2, typename E2, typename = std::enable_if_t<std::is_convertible_v<V2, V> && std::is_convertible_v<E2, E>>> constexpr Result(Result<V2, E2>&& result);
			constexpr Result(const Result&) = default;
			constexpr Result(Result&&) = default;
			~Result() = default;

			constexpr void Expect(const char* err) const;
			constexpr void Expect(const std::string& err) const;

			constexpr E& GetError() &;
			constexpr const E& GetError() const &;
			constexpr E&& GetError() &&;
			
			constexpr V& GetValue() &;
			constexpr const V& GetValue() const&;
			constexpr V&& GetValue() &&;
			
			template<typename T> constexpr V GetValueOr(T&& defaultValue) const &;
			template<typename T> constexpr V GetValueOr(T&& defaultValue) &&;

			constexpr bool IsErr() const noexcept;
			constexpr bool IsOk() const noexcept;

			template<typename F> Result<std::invoke_result_t<F, const V&>, E> constexpr Map(F&& functor) const& noexcept(std::is_nothrow_invocable_v<F, const V&>);
			template<typename F> Result<std::invoke_result_t<F, V&&>, E> constexpr Map(F&& functor) && noexcept(std::is_nothrow_invocable_v<F, V&&>);

			constexpr explicit operator bool() const noexcept;

			constexpr V& operator*() &;
			constexpr const V& operator*() const&;
			constexpr V&& operator*() &&;

			constexpr V* operator->();
			constexpr const V* operator->() const;

			constexpr Result& operator=(const Result&) = default;
			constexpr Result& operator=(Result&&) = default;

		private:
			constexpr void EnsureError() const;
			constexpr void EnsureValue() const;

			static constexpr std::size_t ValueIndex = 0;
			static constexpr std::size_t ErrorIndex = 1;

			std::variant<V, E> m_value;
	};

	template<typename E>
	class Result<void, E>
	{
		static_assert(!std::is_void_v<E>, "error type cannot be void");

		public:
			struct ErrorTag {};
			struct ValueTag {};

			constexpr Result(ResultValue<void> value);
			template<typename T> constexpr Result(ResultError<T> error);
			constexpr Result(ValueTag);
			template<typename... Args> constexpr Result(ErrorTag, Args&&... args);
			template<typename E2, typename = std::enable_if_t<std::is_convertible_v<E2, E>>> constexpr Result(const Result<void, E2>& result);
			template<typename E2, typename = std::enable_if_t<std::is_convertible_v<E2, E>>> constexpr Result(Result<void, E2>&& result);
			constexpr Result(const Result&) = default;
			constexpr Result(Result&&) = default;
			~Result() = default;

			constexpr void Expect(const char* err) const;
			constexpr void Expect(const std::string& err) const;
			
			constexpr E& GetError() &;
			constexpr const E& GetError() const&;
			constexpr E&& GetError() &&;

			constexpr bool IsErr() const noexcept;
			constexpr bool IsOk() const noexcept;

			template<typename F> Result<std::invoke_result_t<F>, E> constexpr Map(F&& functor) const& noexcept(std::is_nothrow_invocable_v<F>);
			template<typename F> Result<std::invoke_result_t<F>, E> constexpr Map(F&& functor) && noexcept(std::is_nothrow_invocable_v<F>);

			constexpr explicit operator bool() const noexcept;

			Result& operator=(const Result&) = default;
			Result& operator=(Result&&) = default;

		private:
			constexpr void EnsureError() const;
			
			std::optional<E> m_error;
	};
}

#include <NazaraUtils/Result.inl>

#endif // NAZARAUTILS_RESULT_HPP
