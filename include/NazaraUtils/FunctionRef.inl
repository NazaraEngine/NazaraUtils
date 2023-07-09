// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <functional>
#include <memory>
#include <utility>

namespace Nz
{
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
	
	template<typename Ret, typename ...Args>
	template<typename... CallArgs, typename>
	Ret FunctionRef<Ret(Args...)>::operator()(CallArgs&&... args) const
	{
		return m_callback(m_functor, std::forward<CallArgs>(args)...);
	}
}
