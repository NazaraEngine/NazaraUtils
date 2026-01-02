// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_PRIVATEIMPL_HPP
#define NAZARAUTILS_PRIVATEIMPL_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/MovablePtr.hpp>
#include <NazaraUtils/TypeTraits.hpp>
#include <cstddef>

namespace Nz
{
	template<typename T, std::size_t BufferSize = 0, std::size_t Alignment = alignof(std::max_align_t)>
	class PrivateImpl
	{
		public:
			template<typename... Args, typename = PreventHiddenCopyMove<PrivateImpl, Args...>> PrivateImpl(Args&&... args);
			PrivateImpl(const PrivateImpl& pimpl);
			PrivateImpl(PrivateImpl&& pimpl) noexcept;
			~PrivateImpl();

			T* Get() noexcept;
			const T* Get() const noexcept;

			operator T* () noexcept;
			operator const T* () const noexcept;

			T& operator*() noexcept;
			const T& operator*() const noexcept;

			T* operator->() noexcept;
			const T* operator->() const noexcept;

			PrivateImpl& operator=(const PrivateImpl& pimpl);
			PrivateImpl& operator=(PrivateImpl&& pimpl) noexcept;

		private:
			struct
			{
				alignas(Alignment) char buffer[BufferSize];
			} m_impl;
	};

	template<typename T, std::size_t Alignment>
	class PrivateImpl<T, 0, Alignment>
	{
		public:
			PrivateImpl(std::nullptr_t);
			PrivateImpl(T* ptr);
			template<typename... Args, typename = PreventHiddenCopyMove<PrivateImpl, Args...>> PrivateImpl(Args&&... args);
			PrivateImpl(const PrivateImpl& pimpl);
			PrivateImpl(PrivateImpl&& pimpl) noexcept = default;
			~PrivateImpl();

			template<typename... Args> T& Emplace(Args&&... args);

			T* Get() noexcept;
			const T* Get() const noexcept;

			bool HasValue() const noexcept;

			void Reset() noexcept;

			operator T* () noexcept;
			operator const T* () const noexcept;

			explicit operator bool() const noexcept;
			
			T& operator*() noexcept;
			const T& operator*() const noexcept;

			T* operator->() noexcept;
			const T* operator->() const noexcept;

			PrivateImpl& operator=(const PrivateImpl& pimpl);
			PrivateImpl& operator=(PrivateImpl&& pimpl) noexcept = default;

		private:
			MovablePtr<T> m_ptr;
	};
}

#include <NazaraUtils/PrivateImpl.inl>

#endif // NAZARAUTILS_CALLONEXIT_HPP
