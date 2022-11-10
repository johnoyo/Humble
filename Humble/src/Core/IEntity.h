#pragma once

#include "HumbleAPI.h"
#include <unordered_map>
#include <string>

namespace HBL {

	struct HBL_API IEntity
	{
		int ID = -1;
		std::unordered_map<std::string, int> components;

		uint32_t m_UUID = 0x0000;
		uint64_t m_Hash = 0x0000;

		std::vector<uint64_t> m_Hashes;
	};

}