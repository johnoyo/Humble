#pragma once

#include "../HumbleAPI.h"
#include "../Utilities/Random.h"

#include <chrono>
#include <stdint.h>
#include <unordered_map>

namespace HBL
{
	struct HBL_API UUID
	{
		UUID() = default;
		UUID(uint64_t uuid) : m_UUID(uuid) {}

		uint64_t m_UUID = 0LU;
		operator uint64_t() const { return m_UUID; }
	};
}

namespace std
{
	template <>
	struct hash<HBL::UUID>
	{
		std::size_t operator()(const HBL::UUID& x) const
		{
			return hash<uint64_t>()((uint64_t)x);
		}
	};
}