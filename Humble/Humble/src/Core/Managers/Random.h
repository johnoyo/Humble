#pragma once

#include <random>
#include "../HumbleAPI.h"

namespace HBL {

	class HBL_API Random
	{
	public:
		static void Init()
		{
			s_RandomEngine.seed(std::random_device()());
		}

		static double Double(double ceiling)
		{
			return (double)(((double)s_Distribution(s_RandomEngine) / (double)std::numeric_limits<uint32_t>::max()) * ceiling);
		}

		static float Float(float ceiling)
		{
			return ((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max()) * ceiling;
		}

		static int Int(int ceiling)
		{
			return (int)(((float)s_Distribution(s_RandomEngine) / (float)std::numeric_limits<uint32_t>::max()) * ceiling);
		}

	private:
		static std::mt19937 s_RandomEngine;
		static std::uniform_int_distribution<std::mt19937::result_type> s_Distribution;
	};

}
