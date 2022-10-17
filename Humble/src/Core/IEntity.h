#pragma once

#include "HumbleAPI.h"
#include <unordered_map>
#include <string>

namespace HBL {

	struct HBL_API IEntity
	{
		int ID = -1;
		std::vector<uint32_t> m_Components;
		std::unordered_map<std::string, int> components;
	};

}