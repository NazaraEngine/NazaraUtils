// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_RESULT_HPP
#define NAZARA_UTILS_RESULT_HPP

#include <Nazara/Prerequisites.hpp>
#include <optional>
#include <string_view>
#include <type_traits>
#include <variant>

namespace Nz
{
	template<typename V>
	struct ResultValue
	{
		V&& value;
	};

	template<typename E>
	struct ResultError
	{
		E&& value;
	};

	template<typename T> ResultValue<std::decay_t<T>> Ok(T&& value);
	template<typename T> ResultError<std::decay_t<T>> Err(T&& err);

	template<typename V, typename E>
	class Result
	{
		static_assert(!std::is_void_v<E>, "error type cannot be void");

		public:
			struct ValueTag {};
			struct ErrorTag {};

			template<typename T> Result(T&& value, std::enable_if_t<std::is_same_v<std::decay_t<T>, V> && !std::is_same_v<V, E>>* = nullptr);
			template<typename T> Result(ResultValue<T>&& value);
			template<typename T> Result(ResultError<T>&& error);
			template<typename... Args> Result(ValueTag, Args&&... args);
			template<typename... Args> Result(ErrorTag, Args&&... args);
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

			template<typename T> Result(ResultError<T>&& error);
			template<typename... Args> Result(ErrorTag, Args&&... args);
			Result(ValueTag);
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

			explicit operator bool() const noexcept;

			Result& operator=(const Result&) = default;
			Result& operator=(Result&&) = default;

		private:
			void EnsureError() const;
			
			std::optional<E> m_error;
	};
}

#include <Nazara/Utils/Result.inl>

#endif // NAZARA_UTILS_RESULT_HPP
