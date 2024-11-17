// Copyright (C) 2024 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <NazaraUtils/MemoryHelper.hpp>
#include <NazaraUtils/TypeTraits.hpp>
#include <new>

namespace Nz
{
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	template<typename... Args, typename>
	PrivateImpl<T, BufferSize, Alignment>::PrivateImpl(Args&&... args)
	{
		PlacementNew(Get(), std::forward<Args>(args)...);
	}

	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	PrivateImpl<T, BufferSize, Alignment>::PrivateImpl(const PrivateImpl& pimpl)
	{
		PlacementNew(Get(), *pimpl.Get());
	}
	
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	PrivateImpl<T, BufferSize, Alignment>::PrivateImpl(PrivateImpl&& pimpl) noexcept
	{
		PlacementNew(Get(), std::move(*pimpl.Get()));
	}
	
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	PrivateImpl<T, BufferSize, Alignment>::~PrivateImpl()
	{
		PlacementDestroy(Get());
	}

	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	T* PrivateImpl<T, BufferSize, Alignment>::Get() noexcept
	{
		static_assert(sizeof(m_impl) >= sizeof(T), "BufferSize is too small to store T");
		static_assert(alignof(decltype(m_impl)) >= alignof(T), "Alignment is too small for T");

		return std::launder(reinterpret_cast<T*>(&m_impl.buffer[0]));
	}
	
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	const T* PrivateImpl<T, BufferSize, Alignment>::Get() const noexcept
	{
		static_assert(sizeof(m_impl) >= sizeof(T), "BufferSize is too small to store T");
		static_assert(alignof(decltype(m_impl)) >= alignof(T), "Alignment is too small for T");

		return std::launder(reinterpret_cast<const T*>(&m_impl.buffer[0]));
	}

	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	PrivateImpl<T, BufferSize, Alignment>::operator T*() noexcept
	{
		return Get();
	}
	
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	PrivateImpl<T, BufferSize, Alignment>::operator const T*() const noexcept
	{
		return Get();
	}

	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	T& PrivateImpl<T, BufferSize, Alignment>::operator*() noexcept
	{
		return *Get();
	}
	
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	const T& PrivateImpl<T, BufferSize, Alignment>::operator*() const noexcept
	{
		return *Get();
	}

	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	T* PrivateImpl<T, BufferSize, Alignment>::operator->() noexcept
	{
		return Get();
	}
	
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	const T* PrivateImpl<T, BufferSize, Alignment>::operator->() const noexcept
	{
		return Get();
	}

	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	auto PrivateImpl<T, BufferSize, Alignment>::operator=(const PrivateImpl& pimpl) -> PrivateImpl&
	{
		PlacementDestroy(Get());
		PlacementNew(Get(), *pimpl.Get());

		return *this;
	}
	
	template<typename T, std::size_t BufferSize, std::size_t Alignment>
	auto PrivateImpl<T, BufferSize, Alignment>::operator=(PrivateImpl&& pimpl) noexcept -> PrivateImpl&
	{
		PlacementDestroy(Get());
		PlacementNew(Get(), std::move(*pimpl.Get()));

		return *this;
	}


	template<typename T, std::size_t Alignment>
	template<typename... Args, typename>
	PrivateImpl<T, 0, Alignment>::PrivateImpl(Args&&... args)
	{
		static_assert(IsComplete_v<T>, "PrivateImpl constructor has to be called in the source file");
		m_ptr = new T(std::forward<Args>(args)...);
	}

	template<typename T, std::size_t Alignment>
	PrivateImpl<T, 0, Alignment>::PrivateImpl(const PrivateImpl& pimpl)
	{
		static_assert(IsComplete_v<T>, "PrivateImpl copy constructor has to be called in the source file");
		m_ptr = new T(*pimpl);
	}

	template<typename T, std::size_t Alignment>
	PrivateImpl<T, 0, Alignment>::~PrivateImpl()
	{
		delete m_ptr;
	}

	template<typename T, std::size_t Alignment>
	T* PrivateImpl<T, 0, Alignment>::Get() noexcept
	{
		return m_ptr;
	}

	template<typename T, std::size_t Alignment>
	const T* PrivateImpl<T, 0, Alignment>::Get() const noexcept
	{
		return m_ptr;
	}

	template<typename T, std::size_t Alignment>
	PrivateImpl<T, 0, Alignment>::operator T* () noexcept
	{
		return Get();
	}

	template<typename T, std::size_t Alignment>
	PrivateImpl<T, 0, Alignment>::operator const T* () const noexcept
	{
		return Get();
	}

	template<typename T, std::size_t Alignment>
	T& PrivateImpl<T, 0, Alignment>::operator*() noexcept
	{
		return *Get();
	}

	template<typename T, std::size_t Alignment>
	const T& PrivateImpl<T, 0, Alignment>::operator*() const noexcept
	{
		return *Get();
	}

	template<typename T, std::size_t Alignment>
	T* PrivateImpl<T, 0, Alignment>::operator->() noexcept
	{
		return Get();
	}

	template<typename T, std::size_t Alignment>
	const T* PrivateImpl<T, 0, Alignment>::operator->() const noexcept
	{
		return Get();
	}

	template<typename T, std::size_t Alignment>
	auto PrivateImpl<T, 0, Alignment>::operator=(const PrivateImpl& pimpl) -> PrivateImpl&
	{
		static_assert(IsComplete_v<T>, "PrivateImpl copy assignation operator has to be called in the source file");

		if constexpr (std::is_copy_assignable_v<T>)
		{
			if (m_ptr)
				*m_ptr = *pimpl;
			else
				m_ptr = new T(*pimpl);
		}
		else
		{
			delete m_ptr;
			m_ptr = new T(*pimpl);
		}

		return *this;
	}
}
