#include <Nazara/Utils/Endianness.hpp>

static_assert(Nz::GetPlatformEndianness() == Nz::Endianness::BigEndian || Nz::GetPlatformEndianness() == Nz::Endianness::LittleEndian);