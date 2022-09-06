// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <Nazara/Utils/Result.hpp>
#include <functional>
#include <stdexcept>

namespace Nz
{
	inline ResultValue<void> Ok()
	{
		return {};
	}

	template<typename T>
	ResultValue<T> Ok(T&& value)
	{
		return ResultValue<T>{ std::move(value) };
	}

	template<typename T> 
	ResultError<T> Err(T&& err)
	{
		return ResultError<T>{ std::move(err) };
	}

	/************************************************************************/

	template<typename V, typename E>
	template<typename T, typename>
	Result<V, E>::Result(T&& value) :
	m_value(std::in_place_index_t<ValueIndex>{}, std::forward<T>(value))
	{
	}
	
	template<typename V, typename E>
	template<typename T>
	Result<V, E>::Result(ResultValue<T>&& value) :
	m_value(std::in_place_index_t<ValueIndex>{}, std::move(value.value))
	{
	}

	template<typename V, typename E>
	template<typename T>
	Result<V, E>::Result(ResultError<T>&& error) :
	m_value(std::in_place_index_t<ErrorIndex>{}, std::move(error.value))
	{
	}

	template<typename V, typename E>
	template<typename... Args>
	Result<V, E>::Result(ValueTag, Args&&... args) :
	m_value(std::in_place_index_t<ValueIndex>{}, std::forward<Args>(args)...)
	{
	}

	template<typename V, typename E>
	template<typename... Args>
	Result<V, E>::Result(ErrorTag, Args&&... args) :
	m_value(std::in_place_index_t<ErrorIndex>{}, std::forward<Args>(args)...)
	{
	}

	template<typename V, typename E>
	template<typename V2, typename E2, typename>
	Result<V, E>::Result(const Result<V2, E2>& result)
	{
		if (result.IsOk())
			m_value.template emplace<ValueIndex>(result.GetValue());
		else
			m_value.template emplace<ErrorIndex>(result.GetError());
	}

	template<typename V, typename E>
	template<typename V2, typename E2, typename>
	Result<V, E>::Result(Result<V2, E2>&& result)
	{
		if (result.IsOk())
			m_value.template emplace<ValueIndex>(std::move(result).GetValue());
		else
			m_value.template emplace<ErrorIndex>(std::move(result).GetError());
	}

	template<typename V, typename E>
	void Result<V, E>::Expect(const char* err) const
	{
		if (IsErr())
			throw std::runtime_error(err);
	}

	template<typename V, typename E>
	void Result<V, E>::Expect(const std::string& err) const
	{
		if (IsErr())
			throw std::runtime_error(err);
	}

	template<typename V, typename E>
	E& Result<V, E>::GetError() &
	{
		EnsureError();
		return std::get<ErrorIndex>(m_value);
	}

	template<typename V, typename E>
	const E& Result<V, E>::GetError() const &
	{
		EnsureError();
		return std::get<ErrorIndex>(m_value);
	}

	template<typename V, typename E>
	E&& Result<V, E>::GetError() &&
	{
		EnsureError();
		return std::get<ErrorIndex>(std::move(m_value));
	}

	template<typename V, typename E>
	bool Result<V, E>::IsErr() const noexcept
	{
		return m_value.index() == ErrorIndex;
	}

	template<typename V, typename E>
	bool Result<V, E>::IsOk() const noexcept
	{
		return m_value.index() == ValueIndex;
	}

	template<typename V, typename E>
	V& Result<V, E>::GetValue() &
	{
		EnsureValue();
		return std::get<ValueIndex>(m_value);
	}

	template<typename V, typename E>
	const V& Result<V, E>::GetValue() const&
	{
		EnsureValue();
		return std::get<ValueIndex>(m_value);
	}
	
	/************************************************************************/
	
	template<typename V, typename E>
	template<typename T>
	V Result<V, E>::GetValueOr(T&& defaultValue) const &
	{
		if (IsOk())
			return std::get<ValueIndex>(m_value);
		else
			return std::forward<T>(defaultValue);
	}

	template<typename V, typename E>
	template<typename T>
	V Result<V, E>::GetValueOr(T&& defaultValue) &&
	{
		if (IsOk())
			return std::get<ValueIndex>(std::move(m_value));
		else
			return std::forward<T>(defaultValue);
	}

	template<typename V, typename E>
	V&& Result<V, E>::GetValue() &&
	{
		EnsureValue();
		return std::get<ValueIndex>(std::move(m_value));
	}

	template<typename V, typename E>
	template<typename F>
	Result<std::invoke_result_t<F, const V&>, E> Result<V, E>::Map(F&& functor) const& noexcept(std::is_nothrow_invocable_v<F, const V&>)
	{
		using MappedVal = std::invoke_result_t<F, const V&>;
		using MappedResult = Result<MappedVal, E>;
		
		if (IsOk())
		{
			if constexpr (!std::is_void_v<MappedVal>)
				return MappedResult(typename MappedResult::ValueTag{}, std::invoke(std::forward<F>(functor), GetValue()));
			else
			{
				std::invoke(std::forward<F>(functor), GetValue());
				return MappedResult(typename MappedResult::ValueTag{});
			}
		}
		else
			return MappedResult(typename MappedResult::ErrorTag{}, GetError());
	}

	template<typename V, typename E>
	template<typename F>
	Result<std::invoke_result_t<F, V&&>, E> Result<V, E>::Map(F&& functor) && noexcept(std::is_nothrow_invocable_v<F, V&&>)
	{
		using MappedVal = std::invoke_result_t<F, V&&>;
		using MappedResult = Result<MappedVal, E>;

		if (IsOk())
		{
			if constexpr (!std::is_void_v<MappedVal>)
				return MappedResult(typename MappedResult::ValueTag{}, std::invoke(std::forward<F>(functor), std::move(*this).GetValue()));
			else
			{
				std::invoke(std::forward<F>(functor), std::move(*this).GetValue());
				return MappedResult(typename MappedResult::ValueTag{});
			}
		}
		else
			return MappedResult(typename MappedResult::ErrorTag{}, std::move(*this).GetError());
	}

	template<typename V, typename E>
	Result<V, E>::operator bool() const noexcept
	{
		return IsOk();
	}

	template<typename V, typename E>
	void Result<V, E>::EnsureError() const
	{
		if (!IsErr())
			throw std::runtime_error("Result is not an error");
	}

	template<typename V, typename E>
	void Result<V, E>::EnsureValue() const
	{
		if (!IsOk())
			throw std::runtime_error("Result is not a value");
	}

	
	template<typename E> 
	Result<void, E>::Result(ResultValue<void>&&)
	{
	}

	template<typename E>
	template<typename T>
	Result<void, E>::Result(ResultError<T>&& error) :
	m_error(std::move(error.value))
	{
	}

	template<typename E>
	Result<void, E>::Result(ValueTag)
	{
	}

	template<typename E>
	template<typename... Args>
	Result<void, E>::Result(ErrorTag, Args&&... args) :
	m_error(std::forward<Args>(args)...)
	{
	}

	template<typename E>
	template<typename E2, typename>
	Result<void, E>::Result(const Result<void, E2>& result)
	{
		if (!result.IsOk())
			m_error.emplace(result.GetError());
	}
	
	template<typename E>
	template<typename E2, typename>
	Result<void, E>::Result(Result<void, E2>&& result)
	{
		if (!result.IsOk())
			m_error.emplace(std::move(result).GetError());
	}
	
	template<typename E>
	E& Result<void, E>::GetError() &
	{
		EnsureError();
		return *m_error;
	}

	template<typename E>
	const E& Result<void, E>::GetError() const &
	{
		EnsureError();
		return *m_error;
	}

	template<typename E>
	E&& Result<void, E>::GetError() &&
	{
		EnsureError();
		return *std::move(m_error);
	}

	template<typename E>
	void Result<void, E>::Expect(const char* err) const
	{
		if (IsErr())
			throw std::runtime_error(err);
	}

	template<typename E>
	void Result<void, E>::Expect(const std::string& err) const
	{
		if (IsErr())
			throw std::runtime_error(err);
	}

	template<typename E>
	bool Result<void, E>::IsErr() const noexcept
	{
		return m_error.has_value();
	}

	template<typename E>
	bool Result<void, E>::IsOk() const noexcept
	{
		return !IsErr();
	}
	
	template<typename E>
	template<typename F>
	Result<std::invoke_result_t<F>, E> Result<void, E>::Map(F&& functor) const& noexcept(std::is_nothrow_invocable_v<F>)
	{
		using MappedVal = std::invoke_result_t<F>;
		using MappedResult = Result<MappedVal, E>;

		if (IsOk())
		{
			if constexpr (!std::is_void_v<MappedVal>)
				return MappedResult(typename MappedResult::ValueTag{}, std::invoke(std::forward<F>(functor)));
			else
			{
				std::invoke(std::forward<F>(functor));
				return MappedResult(typename MappedResult::ValueTag{});
			}
		}
		else
			return MappedResult(typename MappedResult::ErrorTag{}, GetError());
	}

	template<typename E>
	template<typename F>
	Result<std::invoke_result_t<F>, E> Result<void, E>::Map(F&& functor) && noexcept(std::is_nothrow_invocable_v<F>)
	{
		using MappedVal = std::invoke_result_t<F>;
		using MappedResult = Result<MappedVal, E>;

		if (IsOk())
		{
			if constexpr (!std::is_void_v<MappedVal>)
				return MappedResult(typename MappedResult::ValueTag{}, std::invoke(std::forward<F>(functor)));
			else
			{
				std::invoke(std::forward<F>(functor));
				return MappedResult(typename MappedResult::ValueTag{});
			}
		}
		else
			return MappedResult(typename MappedResult::ErrorTag{}, std::move(*this).GetError());
	}

	template<typename E>
	Result<void, E>::operator bool() const noexcept
	{
		return IsOk();
	}

	template<typename E>
	void Result<void, E>::EnsureError() const
	{
		if (!IsErr())
			throw std::runtime_error("Result is not an error");
	}

}
