// Copyright (C) 2022 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Utility library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_FUNCTIONREF_HPP
#define NAZARA_UTILS_FUNCTIONREF_HPP

#include <Nazara/Prerequisites.hpp>
#include <type_traits>

namespace Nz
{
	template<typename T>
	class FunctionRef;

	template<typename Ret, typename... Args>
	class FunctionRef<Ret(Args...)>
	{
		public:
			using Functor = Ret(*)(Args...); //< Only for SFINAE

			template<typename F, typename = std::enable_if_t<std::is_invocable_v<F, Args...>>> FunctionRef(F&& f) noexcept;
			FunctionRef(const FunctionRef&) = default;
			FunctionRef(FunctionRef&&) = default;
			~FunctionRef() = default;

			template<typename... CallArgs, typename = std::enable_if_t<std::is_invocable_v<Functor, CallArgs...>>> Ret operator()(CallArgs&&... args) const;

			FunctionRef& operator=(const FunctionRef&) noexcept = default;
			FunctionRef& operator=(FunctionRef&&) noexcept = default;

		private:
			using Callback = Ret(*)(void* functor, Args...);

			Callback m_callback;
			void* m_functor;
	};

	template<typename Ret, typename... Args>
	FunctionRef(Ret(*)(Args...)) -> FunctionRef<Ret(Args...)>;
}

#include <Nazara/Utils/FunctionRef.inl>

#endif // NAZARA_UTILS_FUNCTIONREF_HPP
