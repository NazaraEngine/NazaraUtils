#include <NazaraUtils/Endianness.hpp>

static_assert(Nz::PlatformEndianness == Nz::Endianness::BigEndian || Nz::PlatformEndianness == Nz::Endianness::LittleEndian);

#ifdef NAZARA_BIG_ENDIAN
static_assert(Nz::HostToBigEndian(std::uint16_t(0xABCD)) == std::uint16_t(0xABCD));
static_assert(Nz::HostToBigEndian(std::uint32_t(0xABCDEF01)) == std::uint32_t(0xABCDEF01));
static_assert(Nz::HostToBigEndian(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0xABCDEF0102030405));
static_assert(Nz::HostToLittleEndian(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
static_assert(Nz::HostToLittleEndian(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
static_assert(Nz::HostToLittleEndian(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));
static_assert(Nz::BigEndianToHost(std::uint16_t(0xABCD)) == std::uint16_t(0xABCD));
static_assert(Nz::BigEndianToHost(std::uint32_t(0xABCDEF01)) == std::uint32_t(0xABCDEF01));
static_assert(Nz::BigEndianToHost(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0xABCDEF0102030405));
static_assert(Nz::LittleEndianToHost(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
static_assert(Nz::LittleEndianToHost(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
static_assert(Nz::LittleEndianToHost(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));
#else
static_assert(Nz::HostToBigEndian(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
static_assert(Nz::HostToBigEndian(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
static_assert(Nz::HostToBigEndian(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));
static_assert(Nz::HostToLittleEndian(std::uint16_t(0xABCD)) == std::uint16_t(0xABCD));
static_assert(Nz::HostToLittleEndian(std::uint32_t(0xABCDEF01)) == std::uint32_t(0xABCDEF01));
static_assert(Nz::HostToLittleEndian(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0xABCDEF0102030405));
static_assert(Nz::BigEndianToHost(std::uint16_t(0xABCD)) == std::uint16_t(0xCDAB));
static_assert(Nz::BigEndianToHost(std::uint32_t(0xABCDEF01)) == std::uint32_t(0x01EFCDAB));
static_assert(Nz::BigEndianToHost(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0x0504030201EFCDAB));
static_assert(Nz::LittleEndianToHost(std::uint16_t(0xABCD)) == std::uint16_t(0xABCD));
static_assert(Nz::LittleEndianToHost(std::uint32_t(0xABCDEF01)) == std::uint32_t(0xABCDEF01));
static_assert(Nz::LittleEndianToHost(std::uint64_t(0xABCDEF0102030405)) == std::uint64_t(0xABCDEF0102030405));
#endif