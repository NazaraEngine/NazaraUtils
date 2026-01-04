// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#include <algorithm>
#include <cassert>
#include <new>
#include <utility>

namespace Nz
{
	namespace Detail
	{
		template<typename T, typename Fallback>
		class FixedVectorBase
		{
			public:
				static constexpr bool HasFallback = true;

			protected:
				void ClearFallback()
				{ 
					fallback.clear();
				}

				template<typename... Args>
				T* EmplaceInFallback(std::size_t index, Args&&... args)
				{
					fallback.emplace(fallback.begin() + index, std::forward<Args>(args)...);
					return &fallback[index];
				}

				template<typename... Args>
				T* EmplaceBackInFallback(Args&&... args)
				{
					fallback.emplace_back(std::forward<Args>(args)...);
					return &fallback.back();
				}

				std::size_t EraseInFallback(std::size_t n)
				{
					auto it = fallback.erase(fallback.begin() + n);
					return std::distance(fallback.begin(), it);
				}
				
				std::size_t EraseInFallback(std::size_t firstIndex, std::size_t lastIndex)
				{
					auto it = fallback.erase(fallback.begin() + firstIndex, fallback.begin() + lastIndex);
					return std::distance(fallback.begin(), it);
				}

				std::size_t GetFallbackCapacity() const
				{ 
					return fallback.capacity();
				}

				T* GetFallbackData(std::size_t n)
				{ 
					return fallback.data() + n;
				}

				const T* GetFallbackData(std::size_t n) const
				{
					return fallback.data() + n;
				}

				std::size_t GetFallbackMaxSize() const
				{ 
					return fallback.max_size();
				}

				std::size_t GetFallbackSize() const
				{ 
					return fallback.size();
				}

				void PopBackInFallback()
				{
					fallback.pop_back();
				}

				void ReserveFallback(std::size_t size)
				{
					fallback.reserve(size);
				}

				void ResizeFallback(std::size_t size)
				{
					fallback.resize(size);
				}

				void ResizeFallback(std::size_t size, const T& value)
				{
					fallback.resize(size, value);
				}

				Fallback fallback;
		};

		template<typename T>
		class FixedVectorBase<T, void>
		{
			public:
				static constexpr bool HasFallback = false;

			protected:
				void ClearFallback()
				{
				}

				template<typename... Args>
				T* EmplaceInFallback(std::size_t /*n*/, Args&&... /*args*/)
				{
					return nullptr;
				}

				template<typename... Args>
				T* EmplaceBackInFallback(Args&&... /*args*/)
				{
					return nullptr;
				}

				std::size_t EraseInFallback(std::size_t /*n*/)
				{
					return 0;
				}

				std::size_t EraseInFallback(std::size_t /*firstIndex*/, std::size_t /*lastIndex*/)
				{
					return 0;
				}

				std::size_t GetFallbackCapacity() const
				{
					return 0;
				}

				T* GetFallbackData(std::size_t /*n*/)
				{
					return nullptr;
				}

				const T* GetFallbackData(std::size_t /*n*/) const
				{
					return nullptr;
				}

				std::size_t GetFallbackMaxSize() const
				{
					return 0;
				}

				std::size_t GetFallbackSize() const
				{
					return 0;
				}

				void PopBackInFallback()
				{
				}

				void ReserveFallback(std::size_t /*size*/)
				{
				}

				void ResizeFallback(std::size_t /*size*/)
				{
				}

				void ResizeFallback(std::size_t /*size*/, const T& /*value*/)
				{
				}
		};
	}

	/*!
	* \ingroup utils
	* \class FixedVector
	* \brief Core class that represents an inplace vector with a compile-time capacity (and thus no allocation required)
	*/

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr FixedVector<T, Capacity, Fallback>::FixedVector() :
	m_size(0)
	{
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr FixedVector<T, Capacity, Fallback>::FixedVector(size_type size, const T& value) :
	FixedVector()
	{
		resize(size, value);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	template<typename InputIt>
	constexpr FixedVector<T, Capacity, Fallback>::FixedVector(InputIt first, InputIt last) :
	FixedVector()
	{
		while (first != last)
			emplace_back(*first++);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	inline constexpr FixedVector<T, Capacity, Fallback>::FixedVector(std::initializer_list<T> init) :
	FixedVector(init.begin(), init.end())
	{
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr FixedVector<T, Capacity, Fallback>::FixedVector(const FixedVector& vec) :
	Base(vec)
	{
		if (!vec.IsUsingFallback())
		{
			m_size = 0;
			for (size_type i = 0; i < vec.size(); ++i)
				push_back(vec[i]);
		}
		else
			m_size = FallbackInUse;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr FixedVector<T, Capacity, Fallback>::FixedVector(FixedVector&& vec) noexcept :
	Base(std::move(vec))
	{
		if (!vec.IsUsingFallback())
		{
			m_size = 0;
			for (size_type i = 0; i < vec.size(); ++i)
				push_back(std::move(vec[i]));
		}
		else
			m_size = FallbackInUse;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	FixedVector<T, Capacity, Fallback>::~FixedVector()
	{
		clear();
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::back() -> reference
	{
		assert(!empty());
		return *data(size() - 1);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::back() const -> const_reference
	{
		assert(!empty());
		return *data(size() - 1);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::begin() noexcept -> iterator
	{
		return iterator(data());
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::begin() const noexcept -> const_iterator
	{
		return const_iterator(data());
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::capacity() const noexcept -> size_type
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
				return Base::GetFallbackCapacity();
		}

		return Capacity;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr void FixedVector<T, Capacity, Fallback>::clear() noexcept
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
			{
				Base::ClearFallback();
				return;
			}
		}

		// can't use resize(0); since it will try to instantiate the default-construction part (which won't compile for classes having no default constructor)
		for (std::size_t i = 0; i < m_size; ++i)
			PlacementDestroy(data(i));

		m_size = 0;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::cbegin() const noexcept -> const_iterator
	{
		return const_iterator(data());
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::cend() const noexcept -> const_iterator
	{
		return const_iterator(data(size()));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::crbegin() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(data(size()));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::crend() const noexcept -> const_reverse_iterator
	{
		return const_reverse_iterator(data());
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr T* FixedVector<T, Capacity, Fallback>::data() noexcept
	{
		return data(0);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr T* FixedVector<T, Capacity, Fallback>::data(size_type n) noexcept
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
				return Base::GetFallbackData(n);
		}

		return std::launder(reinterpret_cast<T*>(&m_data[0]) + n);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr const T* FixedVector<T, Capacity, Fallback>::data() const noexcept
	{
		return data(0);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr const T* FixedVector<T, Capacity, Fallback>::data(size_type n) const noexcept
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
				return Base::GetFallbackData(n);
		}

		return std::launder(reinterpret_cast<const T*>(&m_data[0]) + n);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	template<typename... Args>
	constexpr auto FixedVector<T, Capacity, Fallback>::emplace(const_iterator pos, Args&& ...args) -> iterator
	{
		NazaraAssert(pos >= begin() && pos <= end());
		std::size_t index = std::distance(cbegin(), pos);

		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (CheckFallbackOnGrow())
				return Base::EmplaceInFallback(index, std::forward<Args>(args)...);
		}

		NazaraAssert(m_size < Capacity);
		if (pos < end())
		{
			iterator lastElement = end() - 1;
			PlacementNew(data(m_size), std::move(*lastElement));

			if (data(index) < lastElement)
				std::move_backward(data(index), data(m_size - 1), data(m_size));

			PlacementDestroy(data(index));
		}
		m_size++;

		return PlacementNew(data(index), std::forward<Args>(args)...);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	template<typename... Args>
	constexpr auto FixedVector<T, Capacity, Fallback>::emplace_back(Args&&... args) -> reference
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (CheckFallbackOnGrow())
				return *Base::EmplaceBackInFallback(std::forward<Args>(args)...);
		}

		NazaraAssert(m_size < Capacity);
		return *PlacementNew(data(m_size++), std::forward<Args>(args)...);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr bool FixedVector<T, Capacity, Fallback>::empty() const noexcept
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
				return Base::GetFallbackSize() == 0;
		}

		return m_size == 0;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::end() noexcept -> iterator
	{
		return iterator(data(size()));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::end() const noexcept -> const_iterator
	{
		return const_iterator(data(size()));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::erase(const_iterator pos) -> iterator
	{
		NazaraAssert(pos < end());
		std::size_t index = std::distance(cbegin(), pos);

		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
				return begin() + Base::EraseInFallback(index);
		}

		std::move(begin() + index + 1, end(), begin() + index);
		pop_back();

		return iterator(data(index));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::erase(const_iterator first, const_iterator last) -> iterator
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
			{
				std::size_t firstIndex = std::distance(cbegin(), first);
				std::size_t lastIndex = std::distance(cbegin(), last);
				return begin() + Base::EraseInFallback(firstIndex, lastIndex);
			}
		}

		std::size_t index = std::distance(cbegin(), first);

		if (first == last)
			return begin() + index;

		NazaraAssert(first < last);
		NazaraAssert(first >= begin() && last <= end());

		std::size_t count = std::distance(first, last);

		std::move(begin() + index + count, end(), begin() + index);
		resize(size() - count);

		return iterator(data(index));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::front() noexcept -> reference
	{
		NazaraAssert(!empty());
		return *data();
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::front() const noexcept -> const_reference
	{
		NazaraAssert(!empty());
		return *data();
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::insert(const_iterator pos, const T& value) -> iterator
	{
		return emplace(pos, value);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::insert(const_iterator pos, T&& value) -> iterator
	{
		return emplace(pos, std::move(value));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::max_size() const noexcept -> size_type
	{
		if constexpr (!std::is_same_v<Fallback, void>)
			return Base::GetFallbackMaxSize();
		else
			return FixedCapacity;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::push_back(const T& value) noexcept(std::is_nothrow_copy_constructible<T>::value) -> reference
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (CheckFallbackOnGrow())
				return *Base::EmplaceBackInFallback(value);
		}

		NazaraAssert(m_size < Capacity);
		return *PlacementNew(data(m_size++), value);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::push_back(T&& value) noexcept(std::is_nothrow_move_constructible<T>::value) -> reference
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (CheckFallbackOnGrow())
				return *Base::EmplaceBackInFallback(value);
		}

		NazaraAssert(m_size < Capacity);
		return *PlacementNew(data(m_size++), std::move(value));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr void FixedVector<T, Capacity, Fallback>::pop_back()
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
			{
				Base::PopBackInFallback();
				return;
			}
		}

		NazaraAssert(!empty());
		PlacementDestroy(data(--m_size));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr void FixedVector<T, Capacity, Fallback>::resize(size_type count)
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
			{
				Base::ResizeFallback(count);
				return;
			}
			else if (count > Capacity)
			{
				MoveStorageToFallback(count);
				Base::ResizeFallback(count);
				return;
			}
		}

		NazaraAssert(count <= Capacity);
		if (count > m_size)
		{
			for (std::size_t i = m_size; i < count; ++i)
				PlacementNew(data(i));

			m_size = count;
		}
		else if (count < m_size)
		{
			for (std::size_t i = count; i < m_size; ++i)
				PlacementDestroy(data(i));

			m_size = count;
		}
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr void FixedVector<T, Capacity, Fallback>::resize(size_type count, const value_type& value)
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
			{
				Base::ResizeFallback(count, value);
				return;
			}
			else if (count > Capacity)
			{
				MoveStorageToFallback(count);
				Base::ResizeFallback(count, value);
				return;
			}
		}

		NazaraAssert(count <= Capacity);
		if (count > m_size)
		{
			for (std::size_t i = m_size; i < count; ++i)
				PlacementNew(data(i), value);

			m_size = count;
		}
		else if (count < m_size)
		{
			for (std::size_t i = count; i < m_size; ++i)
				PlacementDestroy(data(i));

			m_size = count;
		}
	}
	
	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr void FixedVector<T, Capacity, Fallback>::reserve(size_type count)
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (count <= Capacity)
				return;

			if (!IsUsingFallback())
				MoveStorageToFallback(count);
			else
				Base::ReserveFallback(count);
		}
		else
		{
			NazaraAssert(count <= Capacity);
		}
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::rbegin() noexcept -> reverse_iterator
	{
		return reverse_iterator(iterator(data(size())));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::rbegin() const noexcept -> const_reverse_iterator
	{
		return reverse_iterator(iterator(data(size())));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::rend() noexcept -> reverse_iterator
	{
		return reverse_iterator(iterator(data()));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::rend() const noexcept -> const_reverse_iterator
	{
		return reverse_iterator(iterator(data()));
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::size() const noexcept -> size_type
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
				return Base::GetFallbackSize();
		}

		return m_size;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::operator[](size_type pos) -> reference
	{
		NazaraAssert(pos < size());
		return *data(pos);
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::operator[](size_type pos) const -> const_reference
	{
		NazaraAssert(pos < size());
		return *data(pos);
	}
	
	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::operator=(const FixedVector& vec) -> FixedVector&
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (vec.IsUsingFallback())
			{
				Base::operator=(vec);
				m_size = vec.m_size;
				return *this;
			}
		}

		clear();
		reserve(vec.size());
		for (size_type i = 0; i < vec.size(); ++i)
			push_back(vec[i]);

		return *this;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr auto FixedVector<T, Capacity, Fallback>::operator=(FixedVector&& vec) noexcept -> FixedVector&
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			// Always move fallback container to steal capacity
			Base::operator=(std::move(vec));
			if (vec.IsUsingFallback())
			{
				m_size = vec.m_size;
				return *this;
			}
		}

		clear();
		reserve(vec.size());
		for (size_type i = 0; i < vec.size(); ++i)
			push_back(std::move(vec[i]));

		return *this;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr bool FixedVector<T, Capacity, Fallback>::CheckFallbackOnGrow()
	{
		if constexpr (!std::is_same_v<Fallback, void>)
		{
			if (IsUsingFallback())
				return true;

			if (m_size >= Capacity)
			{
				MoveStorageToFallback(m_size + 1);
				return true;
			}
			else
				return false;
		}
		else
			return false;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr bool FixedVector<T, Capacity, Fallback>::IsUsingFallback() const
	{
		if constexpr (!std::is_same_v<Fallback, void>)
			return m_size == FallbackInUse;
		else
			return false;
	}

	template<typename T, std::size_t Capacity, typename Fallback>
	constexpr void FixedVector<T, Capacity, Fallback>::MoveStorageToFallback(std::size_t capacity)
	{
		NazaraAssert(!IsUsingFallback());

		Base::ReserveFallback(capacity);
		for (std::size_t i = 0; i < m_size; ++i)
		{
			T* ptr = data(i);
			Base::EmplaceBackInFallback(std::move(*ptr));
			PlacementDestroy(ptr);
		}
		m_size = FallbackInUse;
	}
}
