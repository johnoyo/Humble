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
		static uint64_t splitmix64(uint64_t x)
		{
			x += 0x9e3779b97f4a7c15;
			x = (x ^ (x >> 30))
				* 0xbf58476d1ce4e5b9;
			x = (x ^ (x >> 27))
				* 0x94d049bb133111eb;
			return x ^ (x >> 31);
		}

		std::size_t operator()(const HBL::UUID& x) const
		{
			//static const uint64_t random = std::chrono::steady_clock::now().time_since_epoch().count();
			//static const uint64_t random = HBL::Random::UInt64(0, UINT64_MAX);
			//return splitmix64(x.m_UUID + random);
			return hash<uint64_t>()((uint64_t)x);
		}
	};
}