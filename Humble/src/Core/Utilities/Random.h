#pragma once

#include "../HumbleAPI.h"
#include "../Core.h"

#include <stdint.h>
#include <random>

namespace HBL {

	class HBL_API Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static double Double(double floor, double ceiling)
		{
			ASSERT(floor < ceiling);
			return (double)((((double)s_Distribution(s_RandomEngine) / (double)std::numeric_limits<uint32_t>::max()) * (ceiling - floor) + floor));
		}

		static float Float(float floor, float ceiling)
		{
			ASSERT(floor < ceiling);
			return ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint64_t>::max()) * (ceiling - floor) + floor;
		}

		static int32_t Int32(int32_t floor, int32_t ceiling)
		{
			ASSERT(floor < ceiling);
			return (int32_t)((((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint64_t>::max()) * (ceiling - floor) + floor));
		}

		static int64_t Int64(int64_t floor, int64_t ceiling)
		{
			ASSERT(floor < ceiling);
			return (int64_t)((((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint64_t>::max()) * (ceiling - floor) + floor));
		}

		static uint32_t UInt32(uint32_t floor, uint32_t ceiling)
		{
			ASSERT(floor < ceiling);
			return (uint32_t)((((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint64_t>::max()) * (ceiling - floor) + floor));
		}

		static uint64_t UInt64(uint64_t floor, uint64_t ceiling)
		{
			ASSERT(floor < ceiling);
			return (uint64_t)((((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint64_t>::max()) * (ceiling - floor) + floor));
		}

	private:
		static std::mt19937_64 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937_64::result_type> s_Distribution;
	};

}
