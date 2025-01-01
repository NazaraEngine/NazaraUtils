// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_FUNCTIONREF_HPP
#define NAZARAUTILS_FUNCTIONREF_HPP

#include <NazaraUtils/Prerequisites.hpp>
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

			FunctionRef(std::nullptr_t) noexcept;
			template<typename F, typename = std::enable_if_t<std::is_invocable_r_v<Ret, F&&, Args...> && !std::is_same_v<std::decay_t<F>, FunctionRef>>> FunctionRef(F&& f) noexcept;
			FunctionRef(const FunctionRef&) noexcept = default;
			FunctionRef(FunctionRef&&) noexcept = default;
			~FunctionRef() = default;

			template<typename... CallArgs, typename = std::enable_if_t<std::is_invocable_r_v<Ret, Functor, CallArgs...>>> Ret operator()(CallArgs&&... args) const;

			explicit operator bool() const;

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

#include <NazaraUtils/FunctionRef.inl>

#endif // NAZARAUTILS_FUNCTIONREF_HPP
