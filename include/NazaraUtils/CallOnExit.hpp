// Copyright (C) 2025 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_CALLONEXIT_HPP
#define NAZARAUTILS_CALLONEXIT_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <optional>

#define NAZARA_DEFER(expr) Nz::CallOnExit NazaraSuffixMacro(defer_, __LINE__)([&] { expr })

namespace Nz
{
	template<typename F>
	class CallOnExit
	{
		public:
			CallOnExit() = default;
			CallOnExit(F&& functor);
			CallOnExit(const CallOnExit&) = delete;
			CallOnExit(CallOnExit&&) = delete;
			~CallOnExit();

			void CallAndReset();
			void Reset();

			CallOnExit& operator=(const CallOnExit&) = delete;
			CallOnExit& operator=(CallOnExit&&) = default;

		private:
			std::optional<F> m_functor;
	};

	template<typename F>
	CallOnExit(F) -> CallOnExit<F>;
}

#include <NazaraUtils/CallOnExit.inl>

#endif // NAZARAUTILS_CALLONEXIT_HPP
