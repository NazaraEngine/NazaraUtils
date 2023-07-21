// Copyright (C) 2023 Jérôme "Lynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Engine - Core module"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARA_UTILS_MEMORYPOOL_HPP
#define NAZARA_UTILS_MEMORYPOOL_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/Bitset.hpp>
#include <memory>
#include <vector>

namespace Nz
{
	template<typename T, std::size_t Alignment, bool Const>
	class MemoryPoolIterator;

	template<typename T, std::size_t Alignment = alignof(T)>
	class MemoryPool
	{
		public:
			using const_iterator = MemoryPoolIterator<T, Alignment, true>;
			using iterator = MemoryPoolIterator<T, Alignment, false>;
			friend const_iterator;
			friend iterator;

			class DeferConstruct_t {};
			class NoDestruction_t {};

			MemoryPool(std::size_t blockSize);
			MemoryPool(const MemoryPool&) = delete;
			MemoryPool(MemoryPool&&) noexcept = default;
			~MemoryPool();

			T* Allocate(DeferConstruct_t, std::size_t& index);
			template<typename... Args> T* Allocate(std::size_t& index, Args&&... args);

			void Clear();

			void Free(std::size_t index);
			void Free(std::size_t index, NoDestruction_t);

			std::size_t GetAllocatedEntryCount() const;
			std::size_t GetBlockCount() const;
			std::size_t GetBlockSize() const;
			std::size_t GetFreeEntryCount() const;

			void Reset();

			T* RetrieveFromIndex(std::size_t index);
			const T* RetrieveFromIndex(std::size_t index) const;
			std::size_t RetrieveEntryIndex(const T* data);

			// std interface
			iterator begin();
			const_iterator begin() const;
			const_iterator cbegin() const;
			iterator end();
			const_iterator cend() const;
			const_iterator end() const;
			std::size_t size();

			MemoryPool& operator=(const MemoryPool&) = delete;
			MemoryPool& operator=(MemoryPool&& pool) noexcept = default;

			static constexpr DeferConstruct_t DeferConstruct = {};
			static constexpr std::size_t InvalidIndex = std::numeric_limits<std::size_t>::max();
			static constexpr NoDestruction_t NoDestruction = {};

		private:
			void AllocateBlock();
			T* GetAllocatedPointer(std::size_t blockIndex, std::size_t localIndex);
			const T* GetAllocatedPointer(std::size_t blockIndex, std::size_t localIndex) const;
			std::pair<std::size_t, std::size_t> GetFirstAllocatedEntry() const;
			std::pair<std::size_t, std::size_t> GetFirstAllocatedEntryFromBlock(std::size_t blockIndex) const;
			std::pair<std::size_t, std::size_t> GetNextAllocatedEntry(std::size_t blockIndex, std::size_t localIndex) const;

			using AlignedStorage = std::aligned_storage_t<sizeof(T), Alignment>;

			struct Block
			{
				std::size_t occupiedEntryCount = 0;
				std::unique_ptr<AlignedStorage[]> memory;
				Bitset<UInt64> freeEntries;
				Bitset<UInt64> occupiedEntries; //< Opposite of freeEntries
			};

			std::size_t m_blockSize;
			std::vector<Block> m_blocks;
	};

	template<typename T, std::size_t Alignment, bool Const>
	class MemoryPoolIterator
	{
		using Pool = MemoryPool<T, Alignment>;
		friend Pool;

		public:
			using iterator_category = std::input_iterator_tag;
			using value_type = std::conditional_t<Const, const T, T>;
			using difference_type = std::ptrdiff_t;
			using pointer = value_type*;
			using reference = value_type&;

			MemoryPoolIterator(const MemoryPoolIterator&) = default;
			MemoryPoolIterator(MemoryPoolIterator&&) noexcept = default;

			std::size_t GetIndex() const;

			MemoryPoolIterator& operator=(const MemoryPoolIterator&) = default;
			MemoryPoolIterator& operator=(MemoryPoolIterator&&) noexcept = default;

			MemoryPoolIterator operator++(int);
			MemoryPoolIterator& operator++();

			bool operator==(const MemoryPoolIterator& rhs) const;
			bool operator!=(const MemoryPoolIterator& rhs) const;
			reference operator*() const;

		private:
			MemoryPoolIterator(std::conditional_t<Const, const Pool, Pool>* owner, std::size_t blockIndex, std::size_t localIndex);

			std::size_t m_blockIndex;
			std::size_t m_localIndex;
			std::conditional_t<Const, const Pool, Pool>* m_owner;
	};
}

#include <NazaraUtils/MemoryPool.inl>

#endif // NAZARA_UTILS_MEMORYPOOL_HPP
