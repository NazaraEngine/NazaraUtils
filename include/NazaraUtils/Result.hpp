// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_RESULT_HPP
#define NAZARA_UTILS_RESULT_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <optional>
#include <string_view>
#include <type_traits>
#include <variant>

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


	inline ResultValue<void> Ok();
	template<typename V> auto Ok(V&& value);
	template<typename E> auto Err(E&& err);

	template<typename V, typename E>
	class Result
	{
		static_assert(!std::is_void_v<E>, "error type cannot be void");

		public:
			struct ValueTag {};
			struct ErrorTag {};

			template<typename T, typename = std::enable_if_t<std::is_same_v<std::decay_t<T>, V> && !std::is_same_v<V, E>>> Result(T&& value);
			template<typename T> Result(ResultValue<T> value);
			template<typename T> Result(ResultError<T> error);
			template<typename... Args> Result(ValueTag, Args&&... args);
			template<typename... Args> Result(ErrorTag, Args&&... args);
			template<typename V2, typename E2, typename = std::enable_if_t<std::is_convertible_v<V2, V> && std::is_convertible_v<E2, E>>> Result(const Result<V2, E2>& result);
			template<typename V2, typename E2, typename = std::enable_if_t<std::is_convertible_v<V2, V> && std::is_convertible_v<E2, E>>> Result(Result<V2, E2>&& result);
			Result(const Result&) = default;
			Result(Result&&) = default;
			~Result() = default;

			void Expect(const char* err) const;
			void Expect(const std::string& err) const;

			E& GetError() &;
			const E& GetError() const &;
			E&& GetError() &&;
			
			V& GetValue() &;
			const V& GetValue() const&;
			V&& GetValue() &&;
			
			template<typename T> V GetValueOr(T&& defaultValue) const &;
			template<typename T> V GetValueOr(T&& defaultValue) &&;

			bool IsErr() const noexcept;
			bool IsOk() const noexcept;

			template<typename F> Result<std::invoke_result_t<F, const V&>, E> Map(F&& functor) const& noexcept(std::is_nothrow_invocable_v<F, const V&>);
			template<typename F> Result<std::invoke_result_t<F, V&&>, E> Map(F&& functor) && noexcept(std::is_nothrow_invocable_v<F, V&&>);

			explicit operator bool() const noexcept;

			Result& operator=(const Result&) = default;
			Result& operator=(Result&&) = default;

		private:
			void EnsureError() const;
			void EnsureValue() const;

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

			Result(ResultValue<void> value);
			template<typename T> Result(ResultError<T> error);
			Result(ValueTag);
			template<typename... Args> Result(ErrorTag, Args&&... args);
			template<typename E2, typename = std::enable_if_t<std::is_convertible_v<E2, E>>> Result(const Result<void, E2>& result);
			template<typename E2, typename = std::enable_if_t<std::is_convertible_v<E2, E>>> Result(Result<void, E2>&& result);
			Result(const Result&) = default;
			Result(Result&&) = default;
			~Result() = default;

			void Expect(const char* err) const;
			void Expect(const std::string& err) const;
			
			E& GetError() &;
			const E& GetError() const&;
			E&& GetError() &&;

			bool IsErr() const noexcept;
			bool IsOk() const noexcept;

			template<typename F> Result<std::invoke_result_t<F>, E> Map(F&& functor) const& noexcept(std::is_nothrow_invocable_v<F>);
			template<typename F> Result<std::invoke_result_t<F>, E> Map(F&& functor) && noexcept(std::is_nothrow_invocable_v<F>);

			explicit operator bool() const noexcept;

			Result& operator=(const Result&) = default;
			Result& operator=(Result&&) = default;

		private:
			void EnsureError() const;
			
			std::optional<E> m_error;
	};
}

#include <NazaraUtils/Result.inl>

#endif // NAZARA_UTILS_RESULT_HPP
