// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
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
		m_callback = [](void* functor, Args... args) -> Ret
		{
			return std::invoke(*reinterpret_cast<decltype(std::addressof(f))>(functor), std::forward<Args>(args)...);
		};
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
}
