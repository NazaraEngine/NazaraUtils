// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <functional>
#include <memory>
#include <utility>

namespace Nz
{
	template<typename Ret, typename ...Args>
	FunctionRef<Ret(Args...)>::FunctionRef(std::nullptr_t) noexcept :
	m_functor(nullptr)
	{
	}

	template<typename Ret, typename... Args>
	template<typename F, typename>
	FunctionRef<Ret(Args...)>::FunctionRef(F&& f) noexcept
	{
		m_functor = reinterpret_cast<void*>(std::addressof(f));
		m_callback = &Call<decltype(std::addressof(f))>;
	}
	
	template<typename Ret, typename... Args>
	template<typename... CallArgs, typename>
	Ret FunctionRef<Ret(Args...)>::operator()(CallArgs&&... args) const
	{
		return m_callback(m_functor, std::forward<CallArgs>(args)...);
	}

	template<typename Ret, typename ...Args>
	FunctionRef<Ret(Args...)>::operator bool() const
	{
		return m_functor != nullptr;
	}

	template<typename Ret, typename... Args>
	template<typename Functor>
	Ret FunctionRef<Ret(Args...)>::Call(void* functor, Args... args)
	{
		return std::invoke(*reinterpret_cast<Functor>(functor), std::forward<Args>(args)...);
	}
}
