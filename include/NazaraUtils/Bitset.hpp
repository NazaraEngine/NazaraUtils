// Copyright (C) 2026 Jérôme "SirLynix" Leclercq (lynix680@gmail.com)
// This file is part of the "Nazara Utility Library"
// For conditions of distribution and use, see copyright notice in Config.hpp

#pragma once

#ifndef NAZARAUTILS_BITSET_HPP
#define NAZARAUTILS_BITSET_HPP

#include <NazaraUtils/Prerequisites.hpp>
#include <NazaraUtils/FixedVector.hpp>
#include <NazaraUtils/MathUtils.hpp>
#include <limits>
#include <memory>
#include <string>
#include <type_traits>
#include <vector>

namespace Nz
{
	template<typename Block = UInt32, typename Container = std::vector<Block>>
	class Bitset
	{
		static_assert(std::is_integral<Block>::value && std::is_unsigned<Block>::value, "Block must be a unsigned integral type");

		public:
			class Bit;
			class BitIterator;
			using BitContainer = Container;
			using PointerSequence = std::pair<const void*, std::size_t>; //< Start pointer, bit offset
			struct bits_const_iter_tag;

			constexpr Bitset();
			constexpr explicit Bitset(std::size_t bitCount, bool val);
			constexpr explicit Bitset(const char* bits);
			constexpr Bitset(const char* bits, std::size_t bitCount);
			constexpr Bitset(const Bitset& bitset) = default;
			constexpr explicit Bitset(const std::string_view& bits);
			constexpr explicit Bitset(const std::string& bits);
			template<typename T> constexpr Bitset(T value);
			constexpr Bitset(Bitset&& bitset) noexcept = default;
			~Bitset() noexcept = default;

			template<typename T> constexpr void AppendBits(T bits, std::size_t bitCount);

			constexpr void Clear() noexcept;
			constexpr std::size_t Count() const;
			constexpr void Flip();

			constexpr std::size_t FindFirst() const;
			constexpr std::size_t FindNext(std::size_t bit) const;

			constexpr Block GetBlock(std::size_t i) const;
			constexpr std::size_t GetBlockCount() const;
			constexpr std::size_t GetCapacity() const;
			constexpr std::size_t GetSize() const;

			constexpr void PerformsAND(const Bitset& a, const Bitset& b);
			constexpr void PerformsNOT(const Bitset& a);
			constexpr void PerformsOR(const Bitset& a, const Bitset& b);
			constexpr void PerformsXOR(const Bitset& a, const Bitset& b);

			constexpr bool Intersects(const Bitset& bitset) const;

			constexpr bits_const_iter_tag IterBits() const noexcept;

			constexpr void Reserve(std::size_t bitCount);
			constexpr void Resize(std::size_t bitCount, bool defaultVal = false);

			constexpr void Reset();
			constexpr void Reset(std::size_t bit);

			constexpr void Reverse();

			constexpr void Set(bool val = true);
			constexpr void Set(std::size_t bit, bool val = true);
			constexpr void SetBlock(std::size_t i, Block block);

			constexpr void ShiftLeft(std::size_t pos);
			constexpr void ShiftRight(std::size_t pos);

			constexpr void Swap(Bitset& bitset) noexcept;

			constexpr bool Test(std::size_t bit) const;
			constexpr bool TestAll() const;
			constexpr bool TestAny() const;
			constexpr bool TestNone() const;

			template<typename T> constexpr T To() const;
			std::string ToString() const;

			constexpr void UnboundedReset(std::size_t bit);
			constexpr void UnboundedSet(std::size_t bit, bool val = true);
			constexpr bool UnboundedTest(std::size_t bit) const;

			constexpr PointerSequence Write(const void* ptr, std::size_t bitCount);
			constexpr PointerSequence Write(const PointerSequence& sequence, std::size_t bitCount);

			constexpr Bit operator[](std::size_t index);
			constexpr bool operator[](std::size_t index) const;

			constexpr Bitset operator~() const;

			constexpr Bitset& operator=(const Bitset& bitset) = default;
			constexpr Bitset& operator=(const std::string_view& bits);
			template<typename T> constexpr Bitset& operator=(T value);
			constexpr Bitset& operator=(Bitset&& bitset) noexcept = default;

			constexpr Bitset operator<<(std::size_t pos) const;
			constexpr Bitset& operator<<=(std::size_t pos);

			constexpr Bitset operator>>(std::size_t pos) const;
			constexpr Bitset& operator>>=(std::size_t pos);

			constexpr Bitset& operator&=(const Bitset& bitset);
			constexpr Bitset& operator|=(const Bitset& bitset);
			constexpr Bitset& operator^=(const Bitset& bitset);

			static constexpr Block fullBitMask = std::numeric_limits<Block>::max();
			static constexpr std::size_t bitsPerBlock = BitCount<Block>;
			static constexpr std::size_t npos = std::numeric_limits<std::size_t>::max();

			static constexpr Bitset FromPointer(const void* ptr, std::size_t bitCount, PointerSequence* sequence = nullptr);

			struct bits_const_iter_tag
			{
				constexpr BitIterator begin() const noexcept;
				constexpr BitIterator end() const noexcept;

				const Bitset& bitsetRef;
			};

		private:
			constexpr std::size_t FindFirstFrom(std::size_t blockIndex) const;
			constexpr Block GetLastBlockMask() const;
			constexpr void ResetExtraBits();

			static constexpr std::size_t ComputeBlockCount(std::size_t bitCount);
			static constexpr std::size_t GetBitIndex(std::size_t bit);
			static constexpr std::size_t GetBlockIndex(std::size_t bit);

			Container m_blocks;
			std::size_t m_bitCount;
	};

	template<typename Block, std::size_t Capacity>
	using FixedBitset = Bitset<Block, FixedVector<Block, (Capacity + 7) / 8>>;

	template<typename Block, std::size_t Capacity>
	using HybridBitset = Bitset<Block, HybridVector<Block, (Capacity + 7) / 8>>;

	template<typename Block, typename Container>
	class Bitset<Block, Container>::Bit
	{
		friend Bitset<Block, Container>;

		public:
			constexpr Bit(const Bit& bit) = default;

			constexpr Bit& Flip();
			constexpr Bit& Reset();
			constexpr Bit& Set(bool val = true);
			constexpr bool Test() const;

			template<bool BadCall = true>
			constexpr void* operator&() const;

			constexpr operator bool() const;
			constexpr Bit& operator=(bool val);
			constexpr Bit& operator=(const Bit& bit);

			constexpr Bit& operator|=(bool val);
			constexpr Bit& operator&=(bool val);
			constexpr Bit& operator^=(bool val);
			constexpr Bit& operator-=(bool val);

		private:
			Bit(Block& block, Block mask) :
			m_block(block),
			m_mask(mask)
			{
			}

			Block& m_block;
			Block m_mask;
	};
	
	template<typename Block, class Container>
	class Bitset<Block, Container>::BitIterator
	{
		friend Bitset;

		public:
			using iterator_category = std::input_iterator_tag;
			using value_type = std::size_t;

			constexpr BitIterator(const BitIterator&) = default;
			constexpr BitIterator(BitIterator&&) noexcept = default;

			constexpr BitIterator& operator=(const BitIterator&) = default;
			constexpr BitIterator& operator=(BitIterator&&) noexcept = default;

			constexpr BitIterator operator++(int);
			constexpr BitIterator& operator++();

			constexpr bool operator==(const BitIterator& rhs) const;
			constexpr bool operator!=(const BitIterator& rhs) const;
			constexpr value_type operator*() const;

		private:
			constexpr BitIterator(bits_const_iter_tag bitsetTag, std::size_t bitIndex);

			std::size_t m_bitIndex;
			const Bitset* m_owner;
	};

	template<typename Block, typename Container>
	std::ostream& operator<<(std::ostream& out, const Bitset<Block, Container>& bitset);

	template<typename Block, typename Container>
	constexpr bool operator==(const Bitset<Block, Container>& lhs, const Nz::Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr bool operator!=(const Bitset<Block, Container>& lhs, const Nz::Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr bool operator<(const Bitset<Block, Container>& lhs, const Nz::Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr bool operator<=(const Bitset<Block, Container>& lhs, const Nz::Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr bool operator>(const Bitset<Block, Container>& lhs, const Nz::Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr bool operator>=(const Bitset<Block, Container>& lhs, const Nz::Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr Bitset<Block, Container> operator&(const Bitset<Block, Container>& lhs, const Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr Bitset<Block, Container> operator|(const Bitset<Block, Container>& lhs, const Bitset<Block, Container>& rhs);

	template<typename Block, typename Container>
	constexpr Bitset<Block, Container> operator^(const Bitset<Block, Container>& lhs, const Bitset<Block, Container>& rhs);
}

namespace std
{
	template<typename Block, typename Container>
	constexpr void swap(Nz::Bitset<Block, Container>& lhs, Nz::Bitset<Block, Container>& rhs) noexcept;
}

#include <NazaraUtils/Bitset.inl>

#endif // NAZARAUTILS_BITSET_HPP
